#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <set>
#include <vector>
#include <utility>
#include "SensorSystem.h"

class Algorithm {
public:
    explicit Algorithm(const SensorSystem& sensors);
    std::pair<int, int> decideNextMove(bool finishedCleaning);

private:
    const SensorSystem& sensors;
    std::vector<std::pair<int, int>> path;
    std::set<std::pair<int, int>> cleanedLocations;
    const std::pair<int, int> dockingStation;
};

#endif // ALGORITHM_H