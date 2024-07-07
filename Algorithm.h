#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <set>
#include <vector>
#include <utility>
#include "House.h"

class Algorithm {
public:
    explicit Algorithm(const House::SensorSystem& sensors, std::pair<int, int> dockingStation, int maxBatteryLevel);
    std::pair<int, int> nextStep(bool finishedCleaning);

private:
    const House::SensorSystem& sensors;
    std::vector<std::pair<int, int>> path;
    const std::pair<int, int> dockingStation;
    int maxBatteryLevel;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>> knownLocations;
};

#endif // ALGORITHM_H