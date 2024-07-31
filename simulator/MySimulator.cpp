#include "MySimulator.h"
#include <iostream>	
#include <fstream>
#include <sstream>
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
	std::string steps_log;
	std::ofstream outputFile("output.txt");
	if(!outputFile.is_open())
	{
		throw std::runtime_error("Failed to open output file.");
	}
	std::ostringstream msgStream;
	std::cout << "Simulation started\n";
    House::VacuumCleaner& cleaner = house.getCleaner();
    size_t cnt = 0;
	Step nextMove = Step::Stay;
    while (cnt < cleaner.getMaxAllowedSteps() && nextMove != Step::Finish)
    {
        cnt++;
        std::pair<int, int> curPos = cleaner.getPosition();
        nextMove = algorithm->nextStep();
		std :: cout << "nextMove: " << (int)nextMove << "\n";
		std::pair <int,int> moveTranslation = house.moveTranslation(nextMove);
		steps_log += step2char(nextMove);
		if (nextMove == Step::Finish)
		{
			continue;
		}
		if (nextMove == Step::Stay)
		{
			if (curPos == house.getDockingStation())
        	{
				std::cout << "Charging at docking station (battery level = " << cleaner.batteryLevel() << ")\n";
                cleaner.charge();
			}
			else
			{
				std::cout << "Cleaning at current location\n";
                house.clean();
			}
		}
		else
		{
			std::cout << "Moving\n";
			cleaner.move(moveTranslation.first, moveTranslation.second);
		}

    }
	std::pair<int, int> curPos = cleaner.getPosition();
	std::string status = "";
	if(nextMove == Step::Finish || curPos == house.getDockingStation())
	{
		status = "FINISHED";
	}
	else if(curPos != house.getDockingStation() && this->batteryMeter.getBatteryState() == 0)
	{
		status = "DEAD"; //Should not reach here
	}
	else
	{
		status = "WORKING"; 
	}
	std::string inDock = (curPos == house.getDockingStation()) ? "TRUE" : "FALSE";
	 int score = calcScore(cleaner.getMaxAllowedSteps(), cnt, house.totalDirt(), status, (curPos == house.getDockingStation()));
	//TODO Change format to include steps at the bottom instead of after every step
	std::cout << "Simulation completed\n";
	std::ostringstream logStream;
	logStream << "NumSteps = " << cnt << "\nDirtLeft = " << house.totalDirt() << "\nStatus = " << status << "\nInDock = " << inDock << "\nScore = " << score <<"\nSteps:\n" << steps_log;
	msgLog(outputFile, logStream.str());

	outputFile.close();

}

void MySimulator::readHouseFile(std::string& houseFilePath)
{
	house = House(houseFilePath);
}

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