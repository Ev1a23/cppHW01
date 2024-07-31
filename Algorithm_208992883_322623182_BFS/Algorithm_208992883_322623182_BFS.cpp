#include "Algorithm_208992883_322623182_BFS.h"
#include <iostream>
#include <algorithm>

Algorithm_208992883_322623182_BFS::Algorithm_208992883_322623182_BFS() : MyAlgorithm() {}

Algorithm_208992883_322623182_BFS::~Algorithm_208992883_322623182_BFS() {}

Step Algorithm_208992883_322623182_BFS::nextStep() {
	if(this->algoGrid.empty())
	{
		std::cout << "First step of algo, intializing docking station in algoGrid\n";
		Position dock = MyAlgorithm::Position(0, Direction::North); // TODO
		dock.setDirtLevel(-3);
		this->algoGrid[keyConvert(this->dockingStation)] = dock;
		here = this->dockingStation;
		this->explorationQueue.push(dock);
	}
	std::cout << "totalSteps = " << this->totalSteps << "\n";
    std::cout << "here = (" << here.first << ", " << here.second << ")\n";
    std::cout << "distToDocking = " << algoGrid[keyConvert(here)].getDistToDocking() << "\n";
    std::cout << "current_battery = " << this->batteryMeter->getBatteryState() << "\n";

	this->algoGrid[keyConvert(here)].setDirtLevel(dirtSensor->dirtLevel());
    // Position here_p = this->algoGrid[keyConvert(here)];

	if (here.first == dockingStation.first && here.second == dockingStation.second)
	{
		int stepsToCharge = static_cast<int>((this->maxBatteryLevel - this->batteryMeter->getBatteryState()) / this->rechargeAmount);
		int stepsAfterCharge = this->maxSteps - this->totalSteps - stepsToCharge;
		if (stepsAfterCharge < 0)
		{
			return Step::Finish;
		}
		else if ((this->minDist() * 2 > static_cast<std::size_t>(stepsAfterCharge)) && (static_cast<std::size_t>(stepsAfterCharge) < maxBatteryLevel))
		{ // can't clean anymore
			return Step::Finish;
		}
		else if (batteryMeter->getBatteryState() < maxBatteryLevel)
		{
			std::cout << "Algo decision: Charging\n";
			this->totalSteps++;
			return Step::Stay;
		}
	}

	this->updateLoc();
}

void Algorithm_208992883_322623182_BFS::updateLoc() {
		Direction d;
		Position here_p = this->algoGrid[keyConvert(here)];
		for(int i = 0; i < 4; i++)
		{
			d = static_cast<Direction>(i);
			if(!(this->wallsSensor->isWall(d)))
			{
				Position neighbor_p;
				std::pair<int,int> n = moveTranslation(d);
            	size_t n_key = keyConvert(n);
            	auto it = (this->algoGrid).find(n_key);
				if (it != this->algoGrid.end()) //visited
				{
					neighbor_p = this->algoGrid[n_key];
					if (here_p.getDistToDocking() > neighbor_p.getDistToDocking())
					{
						here_p.setDistToDocking(neighbor_p.getDistToDocking() + 1);
						here_p.setDirectionToDocking(d);
					}
				}
				else
				{
					neighbor_p = Position(here_p.getDistToDocking() + 1 ,static_cast<Direction>((i + 2) % 4));
					this->algoGrid[n_key] = neighbor_p;
					this->explorationQueue.push(neighbor_p);
				}
			}
		}
	}
