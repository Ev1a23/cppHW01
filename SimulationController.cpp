#include "SimulationController.h"
#include <iostream>

SimulationController::SimulationController(const std::string& inputFilePath, const std::string& outputFilePath)
: 	inputFilePath(inputFilePath), 
	outputFilePath(outputFilePath),
	house(House(inputFilePath)), 
	sensors(house),
	algorithm(sensors)
{}
void SimulationController::runSimulation()
{
    VacuumCleaner& cleaner = house.getCleaner();
    int cnt = 0;
    std::cout << "(" << cleaner.getPosition().first << ", " << cleaner.getPosition().second << ") -> ";
    while (house.getTotalDirt() > 0 && cnt < house.getMaxAllowedSteps())
    {
        std::cout << "Total Dirt Level=" << house.getTotalDirt() << "\n";
        cnt++;
        std::pair<int, int> curPos = cleaner.getPosition();
        std::pair<int, int> nextMove = algorithm.decideNextMove(false);
        // std::cout << "Current Position: (" << curPos.first << ", " << curPos.second << "), Next Move: (" << nextMove.first << ", " << nextMove.second << ")\n";

        if (nextMove == house.getDockingStation())
        {
            if (nextMove != curPos)
            {
                std::cout << "Moving to docking station\n";
                cleaner.move(nextMove.first, nextMove.second);
            }
            else
            {
                std::cout << "Charging at docking station (battery level = " << cleaner.batteryLevel() << ")\n";
                cleaner.charge();
            }
        }
        else
        {
            if (nextMove != curPos)
            {
                std::cout << "Moving to next location\n";
                cleaner.move(nextMove.first, nextMove.second);
            }
            else
            {
                std::cout << "Cleaning at current location\n";
                house.cleanPos(nextMove.first, nextMove.second);
            }
        }
        std::cout << "(" << nextMove.first << ", " << nextMove.second << ") -> ";
    }
	std::pair<int, int> curPos = cleaner.getPosition();
	if(house.getTotalDirt() == 0 && curPos != house.getDockingStation())
	{
        std::cout << "\nFinished Cleaning - returning to docking station\n";
		while(cnt < house.getMaxAllowedSteps() && curPos!=house.getDockingStation())
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
}
