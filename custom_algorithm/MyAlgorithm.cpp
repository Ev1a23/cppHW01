#include "MyAlgorithm.h"
#include <iostream>
// #include "House.h"
#include <utility>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <limits>

MyAlgorithm::MyAlgorithm() {}

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
	this->rechargeAmount = this->maxBatteryLevel / 20.0;
}

std::pair<int,int> MyAlgorithm::moveTranslation(Direction directionFromEnum) const
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
				
                if (here_p.getDistToDocking() > neighbor_p.getDistToDocking())
                {
                    here_p.setDistToDocking(neighbor_p.getDistToDocking() + 1);
                    here_p.setDirectionToDocking(d);
                }
            }
            else
            {
                this->algoGrid[n_key] = Position(here_p.getDistToDocking() + 1 ,static_cast<Direction>((i + 2) % 4));
            }
        }
    }
}

std::size_t MyAlgorithm::minDist() const
{ 
	// Find the furthest known point from docking station which is potentially not cleaned
    std::size_t minDist = std::numeric_limits<std::size_t>::max();
    for (auto it = this->algoGrid.begin(); it != this->algoGrid.end(); ++it) 
    {
        if (it->second.getDirtLevel() != 0 && it->second.getDistToDocking() < minDist)
        {
            minDist = it->second.getDistToDocking();
        }
    }
    return minDist;
}

Direction MyAlgorithm::neighborsHandle()
{
    // std::cout << "\t\t" << "neighborsHandle:\n";
    Position herePos = this->algoGrid[keyConvert(here)];
    Direction bestD = herePos.getDirectionToDocking();
    int best = -3; //someone will win -3
    for (int i = 0; i < 4; i++)
    {
        Direction d = static_cast<Direction>(i);
        if (!this->wallsSensor->isWall(d))
        {
            std::pair<int,int> n = moveTranslation(d);
            Position* neighbor = &(this->algoGrid[keyConvert(n)]);
            Direction n_direction = static_cast<Direction>((i + 2) % 4);
            std::size_t n_dist = herePos.getDistToDocking() + 1;
            if (this->algoGrid.find(keyConvert(n)) != this->algoGrid.end())
            {
                if (herePos.getDistToDocking() + 1< neighbor->getDistToDocking())
                {
                    neighbor->setDistToDocking(n_dist);
                    neighbor->setDirectionToDocking(n_direction);
                }
            }
            else
            {
                this->algoGrid[keyConvert(n)] = Position(n_dist, n_direction);
            }
            // std::cout << "\t\t" << "Comparing " << (int)d << "(dirt = " << neighbor->dirtLevel << ")"  << " V.S. " 
            // << int(bestD) << "(dirt = " <<  best << ")" << "\n";
            if (compareDirts(neighbor->getDirtLevel(), best))
            {
                best = neighbor->getDirtLevel();
                bestD = d;
            }
        }
    }
    return bestD;
}

MyAlgorithm::Position::Position(){}

MyAlgorithm::Position::Position(std::size_t distToDocking, Direction directionToDocking) 
: distToDocking(distToDocking), directionToDocking(directionToDocking) {}

void MyAlgorithm::Position::setDirectionToDocking(Direction d)
{
    directionToDocking = d;
}

void MyAlgorithm::Position::setDirtLevel(int dirt)
{
    dirtLevel = dirt;
}

void MyAlgorithm::Position::setDistToDocking(std::size_t dist)
{
    distToDocking = dist;
}

std::size_t MyAlgorithm::Position::getDistToDocking() const
{
    return distToDocking;
}

Direction MyAlgorithm::Position::getDirectionToDocking() const
{
    return directionToDocking;
}

int MyAlgorithm::Position::getDirtLevel() const
{
    return dirtLevel;
}
