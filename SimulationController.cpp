#include "SimulationController.h"
#include <iostream>

SimulationController::SimulationController(const std::string& inputFilePath, const std::string& outputFilePath)
: 	inputFilePath(inputFilePath), 
	outputFilePath(outputFilePath),
	house(House(inputFilePath)), 
	cleaner(10, house.getDockingStation()), 
	sensors(house, cleaner),
	algorithm(sensors)
{}
void SimulationController::runSimulation()
{
    int cnt = 0;
    while (house.getTotalDirt() > 0 && cnt < house.getMaxAllowedSteps())
    {
        cnt++;
        std::pair<int, int> curPos = cleaner.getPosition();
        std::pair<int, int> nextMove = algorithm.decideNextMove(false);
        std::cout << "Current Position: (" << curPos.first << ", " << curPos.second << "), Next Move: (" << nextMove.first << ", " << nextMove.second << ")\n";

        if (nextMove == house.getDockingStation())
        {
            if (nextMove != curPos)
            {
                std::cout << "Moving to docking station\n";
                cleaner.move(nextMove.first, nextMove.second);
            }
            else
            {
                std::cout << "Charging at docking station\n";
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
    }
	std::pair<int, int> curPos = cleaner.getPosition();
	if(house.getTotalDirt() == 0 && curPos != house.getDockingStation())
	{
		while(cnt < house.getMaxAllowedSteps())
		{
			cnt++;
        	std::pair<int, int> nextMove = algorithm.decideNextMove(false);
        	std::cout << "Current Position: (" << curPos.first << ", " << curPos.second << "), Next Move: (" << nextMove.first << ", " << nextMove.second << ")\n";
			cleaner.move(nextMove.first, nextMove.second);
		}
	}
}
