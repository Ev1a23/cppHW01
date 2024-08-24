#include "Algorithm_208992883_322623182_BFS.h"
#include <iostream>
#include <algorithm>

REGISTER_ALGORITHM(Algorithm_208992883_322623182_BFS);

Algorithm_208992883_322623182_BFS::Algorithm_208992883_322623182_BFS() : MyAlgorithm() {}

Algorithm_208992883_322623182_BFS::~Algorithm_208992883_322623182_BFS() {}

Step Algorithm_208992883_322623182_BFS::nextStep() {
    if (this->algoGrid.empty()) {
        initializeGrid();
    }
    // std::cout << "totalSteps = " << this->totalSteps << "\n";
    // std::cout << "here = (" << here.first << ", " << here.second << ")\n";
    // std::cout << "distToDocking = " << algoGrid[keyConvert(here)].getDistToDocking() << "\n";
    // std::cout << "current_battery = " << this->batteryMeter->getBatteryState() << "\n";
	// std::cout << "Total Visited: " << visited.size() << "\n";	

    this->algoGrid[keyConvert(here)].setDirtLevel(dirtSensor->dirtLevel());
    Position& here_p = this->algoGrid[keyConvert(here)];

    if (isAtDockingStation()) {
        Step checkDocking = handleChargingLogic();
        if(checkDocking != Step::North) {
            return checkDocking;
        }
    }

    updateLoc();
	visited.insert(keyConvert(here));


    if ((here_p.getDistToDocking()) + 1 >= std::min(this->batteryMeter->getBatteryState(), this->maxSteps - this->totalSteps)) {
        std::cout << "Algo decision: returning to docking\n";
        return returnToDocking();
    }

    if (this->dirtSensor->dirtLevel() > 0) {
        std::cout << "Algo decision: Cleaning\n";
        this->totalSteps++;
        return Step::Stay;
    }

    return moveTowardsTarget();
}

void Algorithm_208992883_322623182_BFS::initializeGrid() {
    std::cout << "First step of algo, initializing docking station in algoGrid\n";
    Position dock(0, Direction::North);
    dock.setDirtLevel(-3);
    this->algoGrid[keyConvert(this->dockingStation)] = dock;
    here = this->dockingStation;
    bfsQueue.push(here);
    visited.insert(keyConvert(here));
	enqueueNeighbors();
	pathToTarget = calculatePathFromAncestor(here, bfsQueue.front());
	std::cout << "Path to target size: " << pathToTarget.size() << "bfsQueue size: " << bfsQueue.size() << "\n";
}

void Algorithm_208992883_322623182_BFS::updateLoc() {
    Position& here_p = this->algoGrid[keyConvert(here)];
    for (int i = 0; i < 4; i++) {
        Direction d = static_cast<Direction>(i);
        if (!(this->wallsSensor->isWall(d))) {
            std::pair<int,int> n = moveTranslation(d);
            size_t n_key = keyConvert(n);
            auto it = this->algoGrid.find(n_key);
            if (it != this->algoGrid.end()) {
                Position& neighbor_p = it->second;
                if (here_p.getDistToDocking() > neighbor_p.getDistToDocking() + 1) {
                    here_p.setDistToDocking(neighbor_p.getDistToDocking() + 1);
                    here_p.setDirectionToDocking(d);
                }
            } 
			else {
                this->algoGrid[n_key] = Position(here_p.getDistToDocking() + 1, static_cast<Direction>((i + 2) % 4));
            }
        }
    }
}

bool Algorithm_208992883_322623182_BFS::isAtDockingStation() const {
    return here.first == dockingStation.first && here.second == dockingStation.second;
}

Step Algorithm_208992883_322623182_BFS::handleChargingLogic() {
    int stepsToCharge = static_cast<int>((this->maxBatteryLevel - this->batteryMeter->getBatteryState()) / this->rechargeAmount);
    int stepsAfterCharge = this->maxSteps - this->totalSteps - stepsToCharge;
    if (stepsAfterCharge < 0 || 
        (this->minDist() * 2 > static_cast<std::size_t>(stepsAfterCharge) && 
         static_cast<std::size_t>(stepsAfterCharge) < maxBatteryLevel)) {
        return Step::Finish;
    } else if (batteryMeter->getBatteryState() < maxBatteryLevel) {
        std::cout << "Algo decision: Charging\n";
        this->totalSteps++;
        return Step::Stay;
    }
    return Step::North; // dummy return;
}

bool Algorithm_208992883_322623182_BFS::canMoveTo(const std::pair<int, int>& pos) const {
    for (int i = 0; i < 4; ++i) {
        Direction d = static_cast<Direction>(i);
        if (moveTranslation(d) == pos) {
            return !this->wallsSensor->isWall(d);
        }
    }
	std::cout << "Error: canMoveTo() failed\n";
    return false;
}


Step Algorithm_208992883_322623182_BFS::getStepTowards(const std::pair<int, int>& pos) {
    for (int i = 0; i < 4; i++) {
        Direction d = static_cast<Direction>(i);
        if (!this->wallsSensor->isWall(d)) {
            std::pair<int,int> n = moveTranslation(d);
            if (n == pos) {
                return static_cast<Step>(d);
            }
        }
    }
    return Step::Stay;
}

Step Algorithm_208992883_322623182_BFS::returnToDocking() {
    std::cout << "Algo decision: returning to docking\n";
    Direction d = this->algoGrid[keyConvert(here)].getDirectionToDocking();
    here = moveTranslation(d);
    this->totalSteps++;
    return static_cast<Step>(d);
}

void Algorithm_208992883_322623182_BFS::enqueueNeighbors() {
    for (int i = 0; i < 4; i++) {
        Direction d = static_cast<Direction>(i);
        if (!this->wallsSensor->isWall(d)) {
            std::pair<int,int> n = moveTranslation(d);
            size_t n_key = keyConvert(n);
            if (visited.find(n_key) == visited.end()) {
                bfsQueue.push(n);
            }
        }
    }
}

Step Algorithm_208992883_322623182_BFS::moveTowardsTarget() {
	if(!pathToTarget.empty()) 
	{
		Step nextStep = pathToTarget.front();
		pathToTarget.pop_front();
		return nextStep;
	}
    while (!bfsQueue.empty()) {
        std::pair<int, int> target = bfsQueue.front();

        if (visited.find(keyConvert(target)) != visited.end()) {
			bfsQueue.pop();
            continue;
        }
		if(target==here)
		{
			bfsQueue.pop();
			enqueueNeighbors();
			while(!bfsQueue.empty() && visited.find(keyConvert(bfsQueue.front())) != visited.end())
			{
				bfsQueue.pop();
			}
			std::pair<int, int> nextTarget = bfsQueue.front();
			std::deque<Step> curPath = calculatePath(here, nextTarget);
			if(pathToTarget.size() + this->algoGrid[keyConvert(nextTarget)].getDistToDocking() + 1 >= std::min(this->batteryMeter->getBatteryState(), this->maxSteps - this->totalSteps))
			{
				//Not enough steps to reach target
				std::cout << "Algo decision: returning to docking\n";
				pathToTarget = calculatePathToDocking(here);
			}
			else
			{
				std::cout << "Algo decision: calculating new path\n";
				pathToTarget = curPath;
			}
			Step nextStep = pathToTarget.front();
			pathToTarget.pop_front();
			return nextStep;
		}
		else if(here == dockingStation)
		{
			pathToTarget = calculatePath(here, target);
		}
		else
		{
			//should not arrive here
			std::cout << "Error: moveTowardsTarget() failed\n";
			continue;
		}
    }

    return returnToDocking();
}

std::deque<Step> Algorithm_208992883_322623182_BFS::calculatePath(std::pair<int,int> source, std::pair<int,int> target)
{
	std::deque<Step> sourcePathToDocking = calculatePathToDocking(source);
	std::deque<Step> targetPathToDocking = calculatePathToDocking(target);
	std::pair<int,int> sourceCurLoc = source;
	std::pair<int,int> targetCurLoc = target;
	int sourcePathDistToDocking = sourcePathToDocking.size();
	int targetPathDistToDocking = targetPathToDocking.size();
	if(sourcePathDistToDocking != targetPathDistToDocking)
	{
		if(sourcePathDistToDocking < targetPathDistToDocking)
		{
			while(targetPathToDocking.size() > sourcePathDistToDocking)
			{
				targetPathToDocking.pop_front();
			}
		}
		else
		{
			while(sourcePathToDocking.size() > targetPathDistToDocking)
			{
				sourcePathToDocking.pop_front();
			}
		}
	}
	int distToAncestor = 0;
	while(!sourcePathToDocking.empty() && sourceCurLoc != targetCurLoc)
	{
		sourceCurLoc = moveTranslation(static_cast<Direction>(sourcePathToDocking.front()));
		sourcePathToDocking.pop_front();
		targetCurLoc = moveTranslation(static_cast<Direction>(targetPathToDocking.front()));
		targetPathToDocking.pop_front();
		distToAncestor++;
	}
	std::pair<int,int> ancestor = sourceCurLoc;
	sourceCurLoc = source;
	targetCurLoc = target;
	std::deque<Step> pathToDocking = calculatePathToDocking(source);
	std::deque<Step> pathFromDocking = calculatePathFromAncestor(target, ancestor);
	std::deque<Step> path;
	while(sourceCurLoc != ancestor)
	{
		Step s = pathToDocking.front();
		path.push_back(s);
		sourceCurLoc = moveTranslation(static_cast<Direction>(s));
	}
	while(targetCurLoc != ancestor)
	{
		Step s = pathFromDocking.front();
		path.push_back(s);
		targetCurLoc = moveTranslation(static_cast<Direction>(s));
	}
	return path;
}

std::deque<Step> Algorithm_208992883_322623182_BFS::calculatePathToDocking(std::pair<int,int> source)
{
	std::deque<Step> path;
	std::pair<int,int> current = source;
	while(current != dockingStation)
	{
		Direction d = this->algoGrid[keyConvert(current)].getDirectionToDocking();
		path.push_back(static_cast<Step>(d));
		current = moveTranslation(d);
	}
	return path;	
}

std::deque<Step> Algorithm_208992883_322623182_BFS::calculatePathFromAncestor(std::pair<int,int> target, std::pair<int,int> ancestor)
{
	std::deque<Step> path;
	std::pair<int,int> current = target;
	while(current != ancestor)
	{
		Direction d = this->algoGrid[keyConvert(current)].getDirectionToDocking();
		path.push_front(static_cast<Step>((static_cast<int>(d) + 2) % 4));
		current = moveTranslation(d);
	}
	return path;	
}
