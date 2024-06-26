#include "Algorithm.h"
#include <iostream>
#include "VacuumCleaner.h"

Algorithm::Algorithm(const SensorSystem& sensors)
    : sensors(sensors), dockingStation(sensors.getHouse().getDockingStation()) {
		path.push_back(dockingStation);
	}

std::pair<int, int> Algorithm::decideNextMove(bool finishedCleaning)
{
	if(finishedCleaning)
	{
		path.pop_back();
		std::pair<int, int> nextMove = path.back();
		return nextMove;
	}
	const VacuumCleaner& cleaner = sensors.getHouse().getCleaner();

    auto pos = cleaner.getPosition();
     std::cout << "Path length = (" << path.size() - 1 << ")" << " BatteryLevel = (" << cleaner.batteryLevel() << ")\n";
    if (pos == dockingStation && cleaner.batteryLevel() < cleaner.maxBatteryLevel())
    {
        return pos;
    }

    if (static_cast<std::vector<int>::size_type>(cleaner.batteryLevel()) == path.size() - 1 )
    {
		std::cout << "Backtracking\n";
        path.pop_back();
        std::pair<int, int> prevStep = path.back();
        return prevStep;
    }
    
    if (sensors.checkDirt() > 0)
    {
        return pos;
    }

    std::vector<std::pair<int, int>> nonWallLocations = sensors.checkNonWalls();
    int maxDirtLevel = -1;
	std::pair<int, int> nextMove;
	nextMove = path.back();
    for (auto& location : nonWallLocations)
    {
		//std::cout << "Possible location: (" << location.first << ", " << location.second << ")\n";
        int dirtLevel = sensors.getHouse().getDirtLevel(location.first, location.second);
        if (dirtLevel > maxDirtLevel)
        {
            nextMove = location;
            maxDirtLevel = dirtLevel;
        }
    }
	if(nextMove != path.back()){
		path.push_back(nextMove);}
    return nextMove;
}
