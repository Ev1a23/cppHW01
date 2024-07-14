#include "Algorithm.h"
#include <iostream>
#include "House.h"
#include <utility>
#include <random>
#include <algorithm>
#include <unordered_map>

// Algorithm::Algorithm(const House::SensorSystem& sensors, std::pair<int,int> dockingStation, int maxBatteryLevel)
//     : sensors(sensors), dockingStation(dockingStation), maxBatteryLevel(maxBatteryLevel) {
//         std::unordered_map<std::pair<int,int>, std::pair<int,int>>(knownLocations);
//         path.push_back(dockingStation);
//     }

Algorithm::Algorithm() : maxSteps(0), 
                         dirtSensor(DirtSensor::DirtSensor()), 
                         wallsSensor(WallsSensor::WallsSensor()), 
                         batteryMeter(BatteryMeter::BatteryMeter()) {}


Algorithm::Position::Position(std::size_t distToDocking, Direction directionToDocking) 
: distToDocking(distToDocking), directionToDocking(directionToDocking) {}

void Algorithm::setMaxSteps(std::size_t maxSteps)
{
    maxSteps = maxSteps;
}

void Algorithm::setWallsSensor(const WallsSensor& wallsSensor)
{
    this->wallsSensor = wallsSensor;
}

void Algorithm::setDirtSensor(const DirtSensor& dirtSensor)
{
    dirtSensor = dirtSensor;
}

void Algorithm::setBatteryMeter(const BatteryMeter& batteryMeter)
{
    batteryMeter = batteryMeter;
}

void Algorithm::setMaxBatterLevel(std::size_t maxBatteryLevel)
{
    maxBatteryLevel = maxBatteryLevel;
}

void Algorithm::setDockingStation(std::pair<int, int> dockingStation)
{
    dockingStation = dockingStation;
}

static std::pair<int,int> moveTranslation(int directionFromEnum)
{
	switch(directionFromEnum)
	{
		case Direction::North:
			return {-1,0};
		case Direction::East:
			return {0,1};
		case Direction::South:
			return {1,0};
		case Direction::West:
			return {0,-1};
		default:
			return {0,0};
	}
}

static bool compareDirts(int a, int b)
{
    if (a == -3 || b == -3) // one of them is the docking station
    {
        return b == -3;
    }
    if (a == 0) // a is clean
    {
        return false;
    }
    else
    {
        if (a > 0) // a has dirt
        {
            return b == -2 ? true : a > b; 
        }
        else // a is unknown
        {
            return b == 0;
        }
    }
}

void updateHere()
{
    for (int i = 0; i < 4; i++)
    {
        if (!this.wallsSensor.isWall(i))
        {
            std::pair<int,int> n = here + moveTranslation(i);
            Position p = this.algoGrid[n];
            if (p != this.algoGrid.end())
            {
                if (here.distToDocking > p.distToDocking)
                {
                    here.distToDocking = p.distToDocking + 1;
                    here.directionToDocking = i;
                } 
            }
        }
    }
}

std::size_t minDist()
{
    std::size_t minDist = 0;
    for (auto it = this.algoGrid.begin(); it != this.algoGrid.end(); ++it) 
    {
        if (it.seocnd.dirtLevel != 0 && it.second.distToDocking > minDist)
        {
            minDist = it.setDockingStation
        }
    }
    return minDist;
}

Direction neighborsHandle()
{
    Direction d = here.directionToDocking;
    int best = algoGrid[here + moveTranslation(d)].dirtLevel;
    for (int i = 0; i < 4; i++)
    {
        if (!this.wallsSensor.isWall(i))
        {
            Position neighbor;
            std::pair<int,int> n = here + moveTranslation(i);
            neighbor = this.algoGrid[n];
            Direction n_direction = (i + 2) % 4;
            std::size_t n_dist = here.distToDocking + 1;
            if (neighbor != this.algoGrid.end())
            {
                if (here.distToDocking < p.distToDocking)
                {
                    neighbor.distToDocking = n_dist;
                    neighbor.directionToDocking = n_direction;
                }
            }
            else
            {
                neighbor = Position(n_dist, n_direction);
                this.algoGrid[n] = neighbor;
            }
            if (compareDirts(neighbor.dirtLevel, best))
            {
                best = neighbor.dirtLevel;
                d = i;
            }
        }
    }
    return d;
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

    if (this.algoGrid.empty())
    {
        this.setDockingStation({-1,-1});
        Position dock = Algorithm::Position(0, -1);
        dock.dirtLevel = -3;
        this.algoGrid[this.dockingStation] = dock;
        here = this.dockingStation;
    }
    
    ////////////////////////////
    // charging:
    if ((here.first == dockingStation.first && here.second == dockingStation.second)
    {
        if (this.minDist() * 2 > this.maxSteps)
        { // can't clean anymore
            return Step::Finish;
        }
        else if (this.batteryMeter.getBatteryState() < maxBatteryLevel)
        {
            return Step::Stay;
        }
    }

    // if not charging, update info:
    this.updateHere();
    Algorithm::Position nextPosition = this.neighborsHandle(); // use only when exploring

    ////////////////////////////
    // returnning to docking:
    if ((here.distToDocking - 1) >= std::min(this.batteryMeter.getBatteryState(), this.maxSteps))
    {
		std::cout << "Backtracking\n";
        Step res = algoGrid[here].directionToDocking;
        here += moveTranslation(algoGrid[here].directionToDocking);
        return res;
    }
    ////////////////////////////
    // cleaning:
    if (this.dirtSensor() > 0)
    {
        return Step::Stay;
    }
    ////////////////////////////

    // exploring:
    return nextPosition.directionToDocking;
    ////////////////////////////

	// if(finishedCleaning)
	// {
	// 	path.pop_back();
	// 	std::pair<int, int> nextMove = path.back();
	// 	return nextMove;
	// }
	// const HoVacuumCleaner& cleaner = sensors.getHouse().getCleaner();

    // auto pos = cleaner.getPosition();
    // auto pos = path.back();
    // std::cout << "Path length = (" << path.size() - 1 << ")" << " BatteryLevel = (" << sensors.batterySensor() << ")\n";
    // if (pos == dockingStation && sensors.batterySensor() < maxBatteryLevel)
    // {
    //     return pos;
    // }

    // if (static_cast<std::vector<int>::size_type>(sensors.batterySensor()) == path.size() - 1 )
    // {
	// 	std::cout << "Backtracking\n";
    //     path.pop_back();
    //     std::pair<int, int> prevStep = path.back();
    //     return prevStep;
    // }
    
    // if (sensors.dirtSensor() > 0)
    // {
    //     return pos;
    // }

    // std::vector<std::pair<int, int>> nonWallLocations = sensors.wallsSensor();
	// std::pair<int, int> nextMove;
	// if(!nonWallLocations.empty())
    // {
    //     nextMove = nonWallLocations[0];
    // }
    // else
    // {
    //     nextMove = path.back(); // shouldn't get here
    // }
	// if(nextMove != path.back())
    // {
	// 	path.push_back(nextMove);
    // }
    // else
    // {
    //     path.pop_back();
    // }
    // return nextMove;
}
