#include "RandomizedStepAlgorithm.h"
#include <iostream>
#include "House.h"
#include <utility>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <limits>

RandomizedStepAlgorithm::RandomizedStepAlgorithm() {}


Step RandomizedStepAlgorithm::nextStep() {

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
        dock.setDirtLevel(-3);
        this->algoGrid[keyConvert(this->dockingStation)] = dock;
        here = this->dockingStation;
    }
	std::cout << "totalSteps = " << this->totalSteps << "\n";
	
    std::cout << "here = (" << here.first << ", " << here.second << ")\n";
    std::cout << "distToDocking = " << algoGrid[keyConvert(here)].getDistToDocking() << "\n";
    std::cout << "current_battery = " << this->batteryMeter->getBatteryState() << "\n";

    this->algoGrid[keyConvert(here)].setDirtLevel(dirtSensor->dirtLevel());
    Position here_p = this->algoGrid[keyConvert(here)];
    
    ////////////////////////////
    // charging:
    if (here.first == dockingStation.first && here.second == dockingStation.second)
    {
        if (this->minDist() * 2 > (this->maxSteps - this->totalSteps) && this->maxSteps - this->totalSteps < this->maxBatteryLevel)
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

    // if not charging, update info:
    this->updateHere();
    Direction next = neighborsHandle(); // use only when exploring
    ////////////////////////////
    // returning to docking:
    if ((here_p.getDistToDocking()) + 1 >= std::min(this->batteryMeter->getBatteryState(), this->maxSteps - this->totalSteps))
    {
        std::cout << "Algo decision: returning to docking\n";
        Step res = static_cast<Step>(here_p.getDirectionToDocking());
        here = moveTranslation(algoGrid[keyConvert(here)].getDirectionToDocking());
		this->totalSteps++;
        return res;
    }
    ////////////////////////////
    // cleaning:
    if (this->dirtSensor->dirtLevel() > 0)
    {
        std::cout << "Algo decision: Cleaning\n";
		this->totalSteps++;
        return Step::Stay;
    }
    ////////////////////////////

    // exploring:
    std::cout << "Algo decision: Exploring\n";
	this->totalSteps++;
	here = moveTranslation(next);
    return static_cast<Step>(next);

}