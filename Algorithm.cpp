#include "Algorithm.h"
#include <iostream>
#include "House.h"
#include <utility>
#include <random>

// Algorithm::Algorithm(const House::SensorSystem& sensors, std::pair<int,int> dockingStation, int maxBatteryLevel)
//     : sensors(sensors), dockingStation(dockingStation), maxBatteryLevel(maxBatteryLevel) {
//         std::unordered_map<std::pair<int,int>, std::pair<int,int>>(knownLocations);
//         path.push_back(dockingStation);
//     }

void setMaxSteps(std::size_t maxSteps)
{
    this.maxSteps = maxSteps;
}

void setWallsSensor(const WallsSensor& wallsSensor)
{
    this.wallsSensor = wallsSensor;
}

void setDirtSensor(const DirtSensor& dirtSensor)
{
    this.dirtSensor = dirtSensor;
}

void setBatteryMeter(const BatteryMeter& batteryMeter)
{
    this.batteryMeter = batteryMeter;
}

void setMaxBatterLevel(int maxBatteryLevel)
{
    this.maxBatteryLevel = maxBatteryLevel;
}

void setDockingStation(std::pair<int, int> dockingStation)
{
    this.dockingStation = dockingStation;
}

Step Algorithm::nextStep(bool finishedCleaning)
{
    // maintain for each known location its path to the docking station !!
    // maintain a set of locations which you know thier dirt (clean ones as well)

    // logic:
    // if have to go back (aka if len(path_to_dock) is getting too 
    // large wrt to battery level) -> nextStep = move to haed of path_to_dock
    // else
        // if there is dirt on cur location - clean (nextStep = stay)
        // else:
            // update neighbors path to docking station
            // choose where to go by the following priorities:
                // Known Dirty -> Unknown dirt level -> Known Clean
                // 1. dont choose a wall
                // 2. prefer dirty neighbor (if known)
                // 3. otherwise - prefer unknown location over location that you know is cleaned
                // generally if there are two equal choises - prioritize by Direction enum

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
