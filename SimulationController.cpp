#include "SimulationController.h"
#include <iostream>	
#include <fstream>
#include <sstream>

SimulationController::SimulationController(const std::string& inputFilePath)
: 	inputFilePath(inputFilePath), 
	house(House(inputFilePath)), 
	algorithm(house.getSensors(), house.getDockingStation(), house.getCleaner().getMaxBatterySteps())
	{}

void SimulationController::runSimulation()
{
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
    int cnt = 0;
    std::cout << "(" << cleaner.getPosition().first << ", " << cleaner.getPosition().second << ") -> ";
    while (house.getTotalDirt() > 0 && cnt < cleaner.getMaxAllowedSteps())
    {
        std::cout << "Total Dirt Level=" << house.getTotalDirt() << "\n";
        cnt++;
        std::pair<int, int> curPos = cleaner.getPosition();
        std::pair<int, int> nextMove = algorithm.decideNextMove(false);
		msgStream << "(" << cleaner.getPosition().first << ", " << cleaner.getPosition().second << ") -> "
          << "(" << nextMove.first << ", " << nextMove.second << ")";
		msgLog(outputFile, msgStream.str());
		msgStream.str("");
		msgStream.clear();
		//msgLog(outputFile, "(" << cleaner.getPosition().first << ", " << cleaner.getPosition().second << ") -> " << "(" << nextMove.first << ", " << nextMove.second << ")");
        // std::cout << "Current Position: (" << curPos.first << ", " << curPos.second << "), Next Move: (" << nextMove.first << ", " << nextMove.second << ")\n";

		if (nextMove != curPos)
		{
			std::cout << "Moving to docking station\n";
			cleaner.move(nextMove.first, nextMove.second);
		}
		else
		{
			if (nextMove == house.getDockingStation())
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

        // if (nextMove == house.getDockingStation())
        // {
        //     if (nextMove != curPos)
        //     {
        //         std::cout << "Moving to docking station\n";
        //         cleaner.move(nextMove.first, nextMove.second);
        //     }
        //     else
        //     {
        //         std::cout << "Charging at docking station (battery level = " << cleaner.batteryLevel() << ")\n";
        //         cleaner.charge();
        //     }
        // }
        // else
        // {
        //     if (nextMove != curPos)
        //     {
        //         std::cout << "Moving to next location\n";
        //         cleaner.move(nextMove.first, nextMove.second);
        //     }
        //     else
        //     {
        //         std::cout << "Cleaning at current location\n";
        //         house.cleanPos(nextMove.first, nextMove.second);
        //     }
        // }
        std::cout << "(" << nextMove.first << ", " << nextMove.second << ") -> ";
    }
	std::pair<int, int> curPos = cleaner.getPosition();
	if(house.getTotalDirt() == 0 && curPos != house.getDockingStation())
	{
        std::cout << "\nFinished Cleaning - returning to docking station\n";
		while(cnt < cleaner.getMaxAllowedSteps() && curPos!=house.getDockingStation())
		{
			cnt++;
        	std::pair<int, int> nextMove = algorithm.decideNextMove(true);
        	std::cout << "Current Position: (" << curPos.first << ", " << curPos.second << "), Next Move: (" << nextMove.first << ", " << nextMove.second << ")\n";
            curPos = nextMove;
			cleaner.move(nextMove.first, nextMove.second);
		}
		if(curPos == house.getDockingStation())
		{
			std::cout << "Success!! Reached docking station\n";
		}
		else
		{
			std::cout << "Failed to reach docking station due to Max Allowed steps\n";
		}
	}
	else
	{
		std::cout << "Failed to clean all dirt due to Max Allowed steps\n";
	}
	std::cout << "Simulation completed\n";
	std::ostringstream logStream;
	logStream << "Statistics\nTotal Steps Taken: " << cnt <<"\nAmount of Dirt Remaining: " << house.getTotalDirt() << "\nBattery Exausted: " << (cleaner.batteryLevel() == 0 ? "Yes" : "No") << "\nMission Status: " << (house.getTotalDirt() == 0 && curPos == house.getDockingStation() ? "Success" : "Failure");
	msgLog(outputFile, logStream.str());
	outputFile.close();
}

void SimulationController::msgLog(std::ofstream & outputFile, const std::string& msg)
{
	outputFile << msg << std::endl;
	std::cout << msg << std::endl;
}