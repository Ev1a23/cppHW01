#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <utility>
#include "abstract_algorithm.h"
#include <unordered_map>

class MyAlgorithm : public AbstractAlgorithm  {
public:
    MyAlgorithm();
    void setMaxSteps(std::size_t maxSteps);
	void setWallsSensor(const WallsSensor&);
	void setDirtSensor(const DirtSensor&);
	void setBatteryMeter(const BatteryMeter&);
    Step nextStep();

    class Position {
    public:
        Position();
        Position(std::size_t distToDocking, Direction directionToDocking);
        int getDirtLevel() const;
        std::size_t getDistToDocking() const;
        Direction getDirectionToDocking() const;
        void setDirtLevel(int dirtLevel);
        void setDistToDocking(std::size_t dist);
        void setDirectionToDocking(Direction d);

    private:
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
    void updateHere();
    std::pair<int,int> moveTranslation(Direction directionFromEnum) const;
    std::size_t minDist() const;
    Direction neighborsHandle();
};

#endif // ALGORITHM_H