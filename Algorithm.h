#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <set>
#include <vector>
#include <utility>
// #include "House.h"
#include "abstract_algorithm.h"
// #include "enums.h"
// #include "wall_sensor.h"
// #include "dirt_sensor.h"
// #include "battery_meter.h"

class Algorithm : public abstract_algorithm  {
public:
    void setMaxSteps(std::size_t maxSteps);
	void setWallsSensor(const WallsSensor&);
	void setDirtSensor(const DirtSensor&);
	void setBatteryMeter(const BatteryMeter&);
    void setMaxBatterLevel(int maxBatteryLevel);
    void setDockingStation(std::pair<int, int> dockingStation);
    Step nextStep();

    class Position {
    private:
        Position(std::size_t distToDocking, Direction directionToDocking);
        int dirtLevel = -2;
        std::size_t distToDocking = -1;
        Direction directionToDocking = -1;
    }

private:
    const int maxSteps;
    const WallsSensor& wallsSensor;
    const DirtSensor& dirtSensor;
    const BatteryMeter& batteryMeter;
    const std::pair<int, int> dockingStation = {0,0};
    std::size_t maxBatteryLevel;
    std::pair<int,int> here;
    std::unordered_map<std::pair<int,int>, Algorithm::Position> algoGrid;
};

#endif // ALGORITHM_H