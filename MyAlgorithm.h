#ifndef ALGORITHM_H
#define ALGORITHM_H

//#include <set>
#include <vector>
#include <utility>
// #include "House.h"
#include "abstract_algorithm.h"
#include <unordered_map>
// #include "enums.h"
// #include "wall_sensor.h"
// #include "dirt_sensor.h"
// #include "battery_meter.h"

class MyAlgorithm : public AbstractAlgorithm  {
public:
    MyAlgorithm();
    void setMaxSteps(std::size_t maxSteps);
	void setWallsSensor(const WallsSensor&);
	void setDirtSensor(const DirtSensor&);
	void setBatteryMeter(const BatteryMeter&);
    void setMaxBatterLevel(std::size_t maxBatteryLevel);
    void setDockingStation(std::pair<int, int> dockingStation);
    Step nextStep();

    class Position {
    public:
        Position();
        Position(std::size_t distToDocking, Direction directionToDocking);
        int dirtLevel = -2;
        std::size_t distToDocking = 10000000;
        Direction directionToDocking = Direction::North;
    };

private:
    std::size_t maxSteps;
    const WallsSensor* wallsSensor;
    const DirtSensor* dirtSensor;
    const BatteryMeter* batteryMeter;
    std::pair<int, int> dockingStation = {0,0};
    std::size_t maxBatteryLevel;
    std::pair<int,int> here;
    std::unordered_map<size_t, MyAlgorithm::Position> algoGrid;
    // std::unordered_map<std::pair<int, int>, MyAlgorithm::Position, decltype([](const std::pair<int, int>& p) {
    //     return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    // })> algoGrid;
    void updateHere();
    std::pair<int,int> moveTranslation(Direction directionFromEnum);
    std::size_t minDist();
    Direction neighborsHandle();
};

#endif // ALGORITHM_H