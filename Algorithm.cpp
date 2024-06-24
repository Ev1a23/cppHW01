#include "Algorithm.h"
#include <iostream>
#include "VacuumCleaner.h"

Algorithm::Algorithm(const SensorSystem& sensors)
    : sensors(sensors), dockingStation(sensors.getHouse().getDockingStation()) {}

std::pair<int, int> Algorithm::decideNextMove(bool finishedCleaning)
{
	if(!path.empty() && finishedCleaning)
	{
		std::pair<int, int> nextMove = path.back();
		path.pop_back();
		return nextMove;
	}
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
    int maxDirtLevel = -1;
	std::pair<int, int> nextMove;
	if(!path.empty())
	{
    	nextMove = path.back();
	}
	else
	{
		nextMove = pos;
	}
    for (auto& location : nonWallLocations)
    {
		std::cout << "Possible location: (" << location.first << ", " << location.second << ")\n";
        int dirtLevel = sensors.getHouse().getDirtLevel(location.first, location.second);
        if (dirtLevel > maxDirtLevel)
        {
            nextMove = location;
            maxDirtLevel = dirtLevel;
        }
        if (dirtLevel == 0)
        {
            cleanedLocations.insert(location);
        }
    }
	if(!path.empty() && nextMove != path.back())
    {
		path.push_back(nextMove);
	}
    return nextMove;
}
