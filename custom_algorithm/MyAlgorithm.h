#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <utility>
#include "common/AbstractAlgorithm.h"
#include <unordered_map>

class MyAlgorithm : public AbstractAlgorithm  {
public:
    MyAlgorithm();
    void setMaxSteps(std::size_t maxSteps);
	void setWallsSensor(const WallsSensor&);
	void setDirtSensor(const DirtSensor&);
	void setBatteryMeter(const BatteryMeter&);


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

	static size_t keyConvert(std::pair<int,int> pos)
	{
		int i = pos.first;
		int j = pos.second;
		return ((size_t)i) << 32 | (unsigned int) j;
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

protected:
    std::size_t maxSteps;
	std::size_t totalSteps = 0;
    const WallsSensor* wallsSensor;
    const DirtSensor* dirtSensor;
    const BatteryMeter* batteryMeter;
    std::pair<int, int> dockingStation = {0,0};
    std::size_t maxBatteryLevel;
	double rechargeAmount;
    std::pair<int,int> here;
    std::unordered_map<size_t, MyAlgorithm::Position> algoGrid;
    void updateHere();
    std::pair<int,int> moveTranslation(Direction directionFromEnum) const;
	std::pair<int,int> locationTranslation(Direction directionFromEnum, std::pair<int,int> location) const;
    std::size_t minDist() const;
    Direction neighborsHandle();
};

#endif // ALGORITHM_H