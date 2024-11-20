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
    std::cout << "------here = (" << here.first << ", " << here.second << ") batteryLevel = " << this->batteryMeter->getBatteryState() << " distToDocking = "<< algoGrid[keyConvert(here)].getDistToDocking() << "TotalStep Taken = " << this->totalSteps << "------\n"; 
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


    if ((here_p.getDistToDocking()) + 1 > std::min(this->batteryMeter->getBatteryState(), this->maxSteps - this->totalSteps)) {
		pathToTarget = calculatePathToDocking(here);
        return returnToDocking();
    }

    if (this->dirtSensor->dirtLevel() > 0) {
		if(this->dirtSensor->dirtLevel() == 1)
		{
			size_t size_before = bfsQueue.size();
			enqueueNeighbors();
			if(bfsQueue.size() > size_before)
			{
				bfsQueue = std::queue<std::pair<int,int>>();
				enqueueNeighbors();
			}
			if(bfsQueue.empty())
			{
				std::cout << "No more dirt to clean\n";
				pathToTarget = calculatePathToDocking(here);
				bfsQueue.push(dockingStation);
			}
			else
			{
				pathToTarget = calculatePath(here, bfsQueue.front());
				std::cout << "Found dirt at: (" << here.first << ", " << here.second << ")\n";
			}
		}
		std::cout << "Algo decision: cleaning\n";
        this->totalSteps++;
        return Step::Stay;
    }

    Step s = moveTowardsTarget();
	Direction d = static_cast<Direction>(s);
	std::cout << "Taking Step" << static_cast<int>(s) << "\n";
	std::pair<int,int> nextLoc = locationTranslation(d, here);
	std::cout << "Moving to: (" << nextLoc.first << ", " << nextLoc.second << ")\n";
	this->here = nextLoc;
	return s;
}

void Algorithm_208992883_322623182_BFS::initializeGrid() {
    std::cout << "First step of algo, initializing docking station in algoGrid\n";
    Position dock(0, Direction::North);
    dock.setDirtLevel(-3);
    this->algoGrid[keyConvert(this->dockingStation)] = dock;
    here = this->dockingStation;
    visited.insert(keyConvert(here));
	updateLoc();
	enqueueNeighbors();
	pathToTarget = calculatePathFromAncestor(bfsQueue.front(), here);
}

void Algorithm_208992883_322623182_BFS::updateLoc() {
    Position& here_p = this->algoGrid[keyConvert(here)];
    for (int i = 0; i < 4; i++) {
        Direction d = static_cast<Direction>(i);
        if (!(this->wallsSensor->isWall(d))) {
            std::pair<int,int> n = moveTranslation(d);
            size_t n_key = keyConvert(n);
            auto it = this->algoGrid.find(n_key);
			if (it == this->algoGrid.end())
			{
				this->algoGrid[n_key] = Position(here_p.getDistToDocking() + 1, static_cast<Direction>((i + 2) % 4));
			}
            // if (it != this->algoGrid.end()) {
            //     Position& neighbor_p = it->second;
            //     if (here_p.getDistToDocking() >= neighbor_p.getDistToDocking() + 1) {
			// 		std::cout << "Updating position: (" << n.first << ", " << n.second << ") with distToDocking: " << neighbor_p.getDistToDocking() + 1 << "here = (" << here.first << ", " << here.second << ")\n";
            //         here_p.setDistToDocking(neighbor_p.getDistToDocking() + 1);
            //         here_p.setDirectionToDocking(d);
            //     }
            // } 
			// else {
            //     this->algoGrid[n_key] = Position(here_p.getDistToDocking() + 1, static_cast<Direction>((i + 2) % 4));
			// 	std::cout << "New position: (" << n.first << ", " << n.second << ") with distToDocking: " << here_p.getDistToDocking() + 1 << "here = (" << here.first << ", " << here.second << ")\n";
            // }
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
	pathToTarget = calculatePathFromAncestor(bfsQueue.front(), here);
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
				std::cout << "Enqueueing neighbor: (" << n.first << ", " << n.second << ") here= (" <<here.first << "," << here.second << ")\n";
                bfsQueue.push(n);
				this->algoGrid[n_key] = Position(this->algoGrid[keyConvert(here)].getDistToDocking() + 1, static_cast<Direction>((i + 2) % 4));
            }
        }
    }
}

Step Algorithm_208992883_322623182_BFS::moveTowardsTarget() {
	this->totalSteps++;
	if(!pathToTarget.empty()) 
	{
		Step nextStep = pathToTarget.front();
		pathToTarget.pop_front();
		std::cout << "Moving towards target: " << static_cast<int>(nextStep) << "\n";
		return nextStep;
	}
	std::cout << "Path to target is empty\n";
    while (!bfsQueue.empty()) {
        std::pair<int, int> target = bfsQueue.front();
		if(target==here)
		{
			std::cout << "Reached target\n";
			bfsQueue.pop();
			enqueueNeighbors();
			while(!bfsQueue.empty() && visited.find(keyConvert(bfsQueue.front())) != visited.end())
			{
				bfsQueue.pop();
			}
			std::pair<int, int> nextTarget = bfsQueue.front();
			std::cout << "Next target: (" << nextTarget.first << ", " << nextTarget.second << ")\n";
			std::deque<Step> curPath = calculatePath(here, nextTarget);
			std::cout << "Path to target size: " << curPath.size() << "\n";
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
    }

    return returnToDocking();
}

std::deque<Step> Algorithm_208992883_322623182_BFS::calculatePath(std::pair<int,int> source, std::pair<int,int> target)
{
	std::cout << "------Calculating path between points (" << source.first << ", " << source.second << ") and (" << target.first << ", " << target.second << ")------\n";
	for (int i = 0; i < 4; i++)
    {
        Direction d = static_cast<Direction>(i);
        if (!(this->wallsSensor->isWall(d)))
        {
			if (locationTranslation(d, source) == target)
			{
				std::deque<Step> path;
				path.push_back(static_cast<Step>(d));
				return path;
			}
		}
	}
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
	while(!sourcePathToDocking.empty() && sourceCurLoc != targetCurLoc)
	{
		sourceCurLoc = locationTranslation(static_cast<Direction>(sourcePathToDocking.front()), sourceCurLoc);
		sourcePathToDocking.pop_front();
		targetCurLoc = locationTranslation(static_cast<Direction>(targetPathToDocking.front()), targetCurLoc);
		targetPathToDocking.pop_front();
	}
	std::pair<int,int> ancestor = sourceCurLoc;
	sourceCurLoc = source;
	targetCurLoc = target;
	std::deque<Step> pathToDocking = calculatePathToDocking(source);
	std::deque<Step> pathFromAncestor = calculatePathFromAncestor(target, ancestor);
	std::deque<Step> path;
	while(sourceCurLoc != ancestor)
	{
		Step s = pathToDocking.front();
		path.push_back(s);
		pathToDocking.pop_front();
		sourceCurLoc = locationTranslation(static_cast<Direction>(s), sourceCurLoc);
	}
	while(!pathFromAncestor.empty())
	{
		Step s = pathFromAncestor.front();
		path.push_back(s);
		pathFromAncestor.pop_front();
		targetCurLoc = locationTranslation(static_cast<Direction>((static_cast<int>(s)+2)%4), targetCurLoc);
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
		current = locationTranslation(d,current);
	}
	return path;	
}
