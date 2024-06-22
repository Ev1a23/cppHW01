#ifndef SENSOR_SYSTEM_H
#define SENSOR_SYSTEM_H

#include <vector>
#include "House.h"
#include "VacuumCleaner.h"

class SensorSystem {
public:
    SensorSystem(const House& house, const VacuumCleaner& cleaner);
    std::vector<std::pair<int, int>> checkNonWalls() const;
};

#endif // SENSOR_SYSTEM_H