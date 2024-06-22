#include "Algorithm.h"
#include <iostream>
#include "VacuumCleaner.h"

Algorithm::Algorithm(const SensorSystem& sensors)
    : sensors(sensors), dockingStation(sensors.getHouse().getDockingStation()) {}

std::pair<int, int> Algorithm::decideNextMove()
{
	VacuumCleaner cleaner = sensors.getCleaner();
    auto pos = cleaner.getPosition();
    if (pos == dockingStation && cleaner.batteryLevel() < cleaner.maxBatteryLevel())
    {
        return pos;
    }

    if (static_cast<std::vector<int>::size_type>(cleaner.batteryLevel()) == path.size() && !path.empty())
    {
        std::pair<int, int> prevStep = path.back();
        path.pop_back();
        return prevStep;
    }
    
    if (sensors.checkDirt() > 0)
    {
        if (sensors.checkDirt() == 1)
        {
            cleanedLocations.insert(pos);
        }
        return pos;
    }

    cleanedLocations.insert(pos);
    std::vector<std::pair<int, int>> nonWallLocations = sensors.checkNonWalls();
    int minDirtLevel = 100;
    std::pair<int, int> nextMove = pos;
    for (auto& location : nonWallLocations)
    {
		std::cout << "Possible location: (" << location.first << ", " << location.second << ")\n";
        int dirtLevel = sensors.getHouse().getDirtLevel(location.first, location.second);
        if (cleanedLocations.find(location) == cleanedLocations.end() && dirtLevel > 0 && dirtLevel < minDirtLevel)
        {
            nextMove = location;
            minDirtLevel = dirtLevel;
        }
        if (dirtLevel == 0)
        {
            cleanedLocations.insert(location);
        }
    }
    path.push_back(nextMove);
    return nextMove;
}
