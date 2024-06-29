#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <set>
#include <vector>
#include <utility>
#include "House.h"

class Algorithm {
public:
    explicit Algorithm(const House::SensorSystem& sensors, std::pair<int, int> dockingStation, int maxBatteryLevel);
    std::pair<int, int> decideNextMove(bool finishedCleaning);

private:
    const House::SensorSystem& sensors;
    std::vector<std::pair<int, int>> path;
    const std::pair<int, int> dockingStation;
    int maxBatteryLevel;
};

#endif // ALGORITHM_H