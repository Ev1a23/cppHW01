#include "MyAlgorithm.h"
#include <iostream>
#include "House.h"
#include <utility>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <limits>

MyAlgorithm::MyAlgorithm() {}

MyAlgorithm::Position::Position(){}

MyAlgorithm::Position::Position(std::size_t distToDocking, Direction directionToDocking) 
: distToDocking(distToDocking), directionToDocking(directionToDocking) {}

static size_t keyConvert(std::pair<int,int> pos)
{
    int i = pos.first;
    int j = pos.second;
    return ((size_t)i) << 32 | (unsigned int) j;
}

void MyAlgorithm::setMaxSteps(std::size_t maxSteps)
{
    this->maxSteps = maxSteps;
}

void MyAlgorithm::setWallsSensor(const WallsSensor& wallsSensor)
{
    this->wallsSensor = &wallsSensor;
}

void MyAlgorithm::setDirtSensor(const DirtSensor& dirtSensor)
{
    this->dirtSensor = &dirtSensor;
}

void MyAlgorithm::setBatteryMeter(const BatteryMeter& batteryMeter)
{
    this->batteryMeter = &batteryMeter;
    this->maxBatteryLevel = this->batteryMeter->getBatteryState();
}

void MyAlgorithm::setMaxBatterLevel(std::size_t maxBatteryLevel)
{
    this->maxBatteryLevel = maxBatteryLevel;
}

void MyAlgorithm::setDockingStation(std::pair<int, int> dockingStation)
{
    this->dockingStation.first = dockingStation.first;
    this->dockingStation.second = dockingStation.second;
}

std::pair<int,int> MyAlgorithm::moveTranslation(Direction directionFromEnum)
{
    std::pair<int,int> diff;
	switch(directionFromEnum)
	{
		case Direction::North:
			diff = {(-1),0};
            break;
		case Direction::East:
			diff = {0,1};
            break;
		case Direction::South:
			diff = {1,0};
            break;
		case Direction::West:
			diff = {0,-1};
            break;
		default:
			diff = {0,0};
	}
    return {(this->here).first + diff.first, (this->here).second + diff.second};
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

void MyAlgorithm::updateHere()
{
    Direction d;
    Position here_p = this->algoGrid[keyConvert(here)];
    for (int i = 0; i < 4; i++)
    {
        d = static_cast<Direction>(i);
        if (!(this->wallsSensor->isWall(d)))
        {
            std::pair<int,int> n = moveTranslation(d);
            size_t n_key = keyConvert(n);
            auto it = (this->algoGrid).find(n_key);
            if (it != this->algoGrid.end())
            {
                Position neighbor_p = this->algoGrid[n_key];
                if (here_p.distToDocking > neighbor_p.distToDocking)
                {
                    here_p.distToDocking = neighbor_p.distToDocking + 1;
                    here_p.directionToDocking = d;
                }
            }
            else
            {
                this->algoGrid[n_key] = Position(here_p.distToDocking + 1 ,static_cast<Direction>((i + 2) % 4));
            }
        }
    }
}

std::size_t MyAlgorithm::minDist()
{ 
	// Find the furthest known point from docking station which is potentially not cleaned
    std::size_t minDist = std::numeric_limits<std::size_t>::max();
    for (auto it = this->algoGrid.begin(); it != this->algoGrid.end(); ++it) 
    {
        if (it->second.dirtLevel != 0 && it->second.distToDocking < minDist)
        {
            minDist = it->second.distToDocking;
        }
    }
    return minDist;
}

Direction MyAlgorithm::neighborsHandle()
{
    Position herePos = this->algoGrid[keyConvert(here)];
    Direction bestD = herePos.directionToDocking;
    int best = -3; //someone will win -3
    for (int i = 0; i < 4; i++)
    {
        Direction d = static_cast<Direction>(i);
        if (!(*(this->wallsSensor)).isWall(d))
        {
            std::pair<int,int> n = moveTranslation(d);
            Position* neighbor = &(this->algoGrid[keyConvert(n)]);
            Direction n_direction = static_cast<Direction>((i + 2) % 4);
            std::size_t n_dist = herePos.distToDocking + 1;
            if (this->algoGrid.find(keyConvert(n)) != this->algoGrid.end())
            {
                if (herePos.distToDocking + 1< neighbor->distToDocking )
                {
                    neighbor->distToDocking = n_dist;
                    neighbor->directionToDocking = n_direction;
                }
            }
            else
            {
                this->algoGrid[keyConvert(n)] = Position(n_dist, n_direction);
            }
            if (compareDirts(neighbor->dirtLevel, best))
            {
                best = neighbor->dirtLevel;
                bestD = d;
            }
        }
    }
    return bestD;
}

Step MyAlgorithm::nextStep()
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

    if (this->algoGrid.empty())
    {
        std::cout << "First step of algo, intializing docking station in algoGrid\n";
        Position dock = MyAlgorithm::Position(0, Direction::North); // TODO
        dock.dirtLevel = -3;
        this->algoGrid[keyConvert(this->dockingStation)] = dock;
        here = this->dockingStation;
    }
	std::cout << "here = (" << here.first << ", " << here.second << ") distToDocking = " << algoGrid[keyConvert(here)].distToDocking << "\n";
    Position here_p = this->algoGrid[keyConvert(here)];
    
    ////////////////////////////
    // charging:
    if ((here.first == dockingStation.first && here.second == dockingStation.second) &&
        batteryMeter->getBatteryState() < maxBatteryLevel)
    {
        std::cout << "Algo decision: Charging\n";
        if (this->minDist() * 2 > this->maxSteps)
        { // can't clean anymore
            return Step::Finish;
        }
        else if (batteryMeter->getBatteryState() < maxBatteryLevel)
        {
            return Step::Stay;
        }
    }

    // if not charging, update info:
    this->updateHere();
    Direction next = neighborsHandle(); // use only when exploring

    ////////////////////////////
    // returning to docking:
    if ((here_p.distToDocking) >= std::min(this->batteryMeter->getBatteryState(), this->maxSteps) + 1)
    {
        std::cout << "Algo decision: returning to docking\n";
        std::cout << "current_battery = " << this->batteryMeter->getBatteryState() << "\n";
        std::cout << "maxSteps = " << this->maxSteps << "\n";
        std::cout << "here_p.distToDocking - 1 = " << here_p.distToDocking - 1 << "\n";

        Step res = static_cast<Step>(here_p.directionToDocking);
        here = moveTranslation(algoGrid[keyConvert(here)].directionToDocking);
        return res;
    }
    ////////////////////////////
    // cleaning:
    if ((*(this->dirtSensor)).dirtLevel() > 0)
    {
        std::cout << "Algo decision: Cleaning:\n";
        return Step::Stay;
    }
    ////////////////////////////

    // exploring:
    std::cout << "Algo decision: Exploring:\n";
	here = moveTranslation(next);
    return static_cast<Step>(next);

}
