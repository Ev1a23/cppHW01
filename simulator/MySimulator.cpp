#include "MySimulator.h"
// #include <iostream>	
#include <fstream>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include "enums.h"


MySimulator::MySimulator() : dirtSensor(&house), wallsSensor(&house), batteryMeter(&house){}

static std::string step2char(Step step)
{
	switch (step)
	{
		case Step::North:
			return "N";
		case Step::East:
			return "E";
		case Step::South:
			return "S";
		case Step::West:
			return "W";
		case Step::Stay:
			return "s";
		case Step::Finish:
			return "F";
		default:
			return "";
	}
}

void MySimulator::run()
{
	results.setValues(0, house.totalDirt(), "WORKING", "True", -1, std::string(""));
	std::cout << "Simulation started\n";
    House::VacuumCleaner& cleaner = house.getCleaner();
    size_t cnt = 0;
	Step nextMove = Step::Stay;
    while (cnt < cleaner.getMaxAllowedSteps() && nextMove != Step::Finish)
    {
		pthread_testcancel();  // Check for cancellation (in addition to built in cancellation points)
        std::pair<int, int> curPos = cleaner.getPosition();
        nextMove = algorithm->nextStep();
		std::pair <int,int> moveTranslation = house.moveTranslation(nextMove);
		if (nextMove == Step::Finish)
		{
			results.stepsLog += step2char(nextMove);
			break;
		}
		if (nextMove == Step::Stay)
		{
			if (curPos == house.getDockingStation())
        	{
				// std::cout << "Charging at docking station (battery level = " << cleaner.batteryLevel() << ")\n";
                cleaner.charge();
			}
			else
			{
				// std::cout << "Cleaning at current location\n";
                house.clean();
			}
		}
		else
		{
			// std::cout << "Moving\n";
			cleaner.move(moveTranslation.first, moveTranslation.second);
			results.inDock = (curPos == house.getDockingStation()) ? "TRUE" : "FALSE";
		}
		results.numSteps ++;
		results.stepsLog += step2char(nextMove);
		results.dirtLeft = house.totalDirt();
    }
	std::pair<int, int> curPos = cleaner.getPosition();
	if(nextMove == Step::Finish || curPos == house.getDockingStation())
	{
		results.status = "FINISHED";
	}
	else if(curPos != house.getDockingStation() && this->batteryMeter.getBatteryState() == 0)
	{
		results.status = "DEAD"; //Should not reach here
	}
	else
	{
		results.status = "WORKING"; 
	}
	results.inDock = (curPos == house.getDockingStation()) ? "TRUE" : "FALSE";
	results.score = calcScore(house.getMaxSteps(), results.numSteps, results.dirtLeft, results.status, (curPos == house.getDockingStation()));
	//TODO Change format to include steps at the bottom instead of after every step
	std::cout << "Simulation completed\n";
	// return resutls.score;
}

// void MySimulator::readHouseFile(std::string& houseFilePath)
// {
// 	house = House(houseFilePath);
// }

void MySimulator::setAlgorithm(AbstractAlgorithm& algorithm)
{
	this->algorithm = &algorithm;
	this->algorithm->setMaxSteps(house.getCleaner().getMaxAllowedSteps());
	this->algorithm->setWallsSensor(this->wallsSensor);
	this->algorithm->setDirtSensor(this->dirtSensor);
	this->algorithm->setBatteryMeter(this->batteryMeter);
}

void MySimulator::msgLog(std::ofstream & outputFile, const std::string& msg) const
{
	outputFile << msg;
	std::cout << msg << std::endl;
}