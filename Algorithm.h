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
        int dirtLevel = -1;
        int distToDocking = 

    }

private:
    const int maxSteps;
    const WallsSensor& wallsSensor;
    const DirtSensor& dirtSensor;
    const BatteryMeter& batteryMeter;

    // std::vector<std::pair<int, int>> path;
    const std::pair<int, int> dockingStation;
    int maxBatteryLevel;
};

#endif // ALGORITHM_H