#include "Algorithm.h"
#include <iostream>
#include "House.h"
#include <utility>
#include <random>

Algorithm::Algorithm(const House::SensorSystem& sensors, std::pair<int,int> dockingStation, int maxBatteryLevel)
    : sensors(sensors), dockingStation(dockingStation), maxBatteryLevel(maxBatteryLevel) {
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
	// const HoVacuumCleaner& cleaner = sensors.getHouse().getCleaner();

    // auto pos = cleaner.getPosition();
    auto pos = path.back();
    std::cout << "Path length = (" << path.size() - 1 << ")" << " BatteryLevel = (" << sensors.batterySensor() << ")\n";
    if (pos == dockingStation && sensors.batterySensor() < maxBatteryLevel)
    {
        return pos;
    }

    if (static_cast<std::vector<int>::size_type>(sensors.batterySensor()) == path.size() - 1 )
    {
		std::cout << "Backtracking\n";
        path.pop_back();
        std::pair<int, int> prevStep = path.back();
        return prevStep;
    }
    
    if (sensors.dirtSensor() > 0)
    {
        return pos;
    }

    std::vector<std::pair<int, int>> nonWallLocations = sensors.wallsSensor();
	std::pair<int, int> nextMove;
	if(!nonWallLocations.empty())
    {
        nextMove = nonWallLocations[0];
    }
    else
    {
        nextMove = path.back(); // shouldn't get here
    }
	if(nextMove != path.back())
    {
		path.push_back(nextMove);
    }
    else
    {
        path.pop_back();
    }
    return nextMove;
}
