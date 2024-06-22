#include "SimulationController.h"
#include <iostream>

SimulationController::SimulationController(const std::string& inputFilePath, const std::string& outputFilePath)
    : inputFilePath(inputFilePath), outputFilePath(outputFilePath)
{
    house = FileManagement::loadHouse(inputFilePath);
    // Assume getDockingStation() returns the needed position
    cleaner = VacuumCleaner(10, house.getDockingStation());
    sensors = SensorSystem(house, cleaner);
    algorithm = Algorithm(sensors);
}

void SimulationController::runSimulation()
{
    int cnt = 0;
    while (house.getTotalDirt() > 0 && cnt < house.getMaxAllowedSteps())
    {
        cnt++;
        std::pair<int, int> curPos = cleaner.getPosition();
        std::pair<int, int> nextMove = algorithm.decideNextMove();
        std::cout << "Current Position: (" << curPos.first << ", " << curPos.second << "), Next Move: (" << nextMove.first << ", " << nextMove.second << ")\n";

        if (nextMove == house.getDockingStation())
        {
            if (nextMove != curPos)
            {
                std::cout << "Moving to docking station\n";
                cleaner.move(nextMove);
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
                cleaner.move(nextMove);
            }
            else
            {
                std::cout << "Cleaning at current location\n";
                house.cleanPos(nextMove.first, nextMove.second);
            }
        }
    }
}
