#include "MySimulator.h"
#include <iostream>	
#include <fstream>
#include <sstream>
#include "enums.h"


MySimulator::MySimulator() : dirtSensor(&house), wallsSensor(&house), batteryMeter(&house)
// : house(),
//   algorithm()
{
    //std::cout << "dirtSensor's house address = " << dirtSensor->house << "\n";
}

void MySimulator::run()
{
	// std::cout << house->getCleaner().getPosition().first << "\n";
	std::ofstream outputFile("output.txt");
	if(!outputFile.is_open())
	{
		throw std::runtime_error("Failed to open output file.");
	}
	std::ostringstream msgStream;
	msgStream << "Steps Performed (Current Location -> Next Location)";
	msgLog(outputFile, msgStream.str());
	msgStream.str("");
	msgStream.clear();
	std::cout << "Simulation started\n";
    House::VacuumCleaner& cleaner = house.getCleaner();
    // std::cout << &cleaner << "\n";
    size_t cnt = 0;
	Step nextMove = Step::Stay;
    std::cout << "(" << cleaner.getPosition().first << ", " << cleaner.getPosition().second << ") -> \n";
    while (cnt < cleaner.getMaxAllowedSteps() && nextMove != Step::Finish)
    {
        // std::cout << "Total Dirt Level=" << house->getTotalDirt() << "\n";
        cnt++;
        std::pair<int, int> curPos = cleaner.getPosition();
		std::cout << "Simulator: query algorithm step\n";
        nextMove = algorithm->nextStep();
		std::cout << "Simulator: query algorithm step:\n";
		std::pair <int,int> moveTranslation = house.moveTranslation(nextMove);
		msgStream << "(" << curPos.first << ", " << curPos.second << ") -> "
          << "(" << curPos.first + moveTranslation.first  << ", " << curPos.first + moveTranslation.first << ")";
		msgLog(outputFile, msgStream.str());
		msgStream.str("");
		msgStream.clear();
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
			cleaner.move(curPos.first + moveTranslation.first, curPos.second + moveTranslation.second);
		}

        std::cout << "(" << curPos.first + moveTranslation.first << ", " << curPos.second + moveTranslation.second << ") -> ";
    }
	std::pair<int, int> curPos = cleaner.getPosition();
	std::string status = "";
	if(curPos == house.getDockingStation() && nextMove == Step::Finish)
	{
		status = "FINISHED";
	}
	else if(cnt == cleaner.getMaxAllowedSteps() && curPos != house.getDockingStation())
	{
		status = "WORKING";
	}
	else
	{
		status = "DEAD"; //Should not reach here
	}
	//TODO Change format to include steps at the bottom instead of after every step
	std::cout << "Simulation completed\n";
	std::ostringstream logStream;
	logStream << "NumSteps= " << cnt << "\nStatus= " << status;
	msgLog(outputFile, logStream.str());

	outputFile.close();
}

void MySimulator::readHouseFile(std::string& houseFilePath)
{
	// House h = House(houseFilePath);
	// house = &h;
	house = House(houseFilePath);
	std::cout << "Accessing dirt level in MySimulator: dirtLevel = " << dirtSensor.dirtLevel() << "\n";
}

void MySimulator::setAlgorithm(AbstractAlgorithm& algorithm)
{
	this->algorithm = &algorithm;
	this->algorithm->setMaxSteps(house.getCleaner().getMaxAllowedSteps());
	this->algorithm->setWallsSensor(this->wallsSensor);
	this->algorithm->setDirtSensor(this->dirtSensor);
	this->algorithm->setBatteryMeter(this->batteryMeter);
}

void MySimulator::msgLog(std::ofstream & outputFile, const std::string& msg)
{
	outputFile << msg << std::endl;
	std::cout << msg << std::endl;
}


