#ifndef SENSOR_SYSTEM_H
#define SENSOR_SYSTEM_H

#include <vector>
#include "House.h"
#include "VacuumCleaner.h"

class SensorSystem {
public:
    SensorSystem(const House& house, const VacuumCleaner& cleaner);
    std::vector<std::pair<int, int>> checkNonWalls() const;
    int batteryStatus() const;
	int checkDirt() const;
	House getHouse() const;
	VacuumCleaner getCleaner() const;

private:
    const House& house;
    const VacuumCleaner& cleaner;
};

#endif // SENSOR_SYSTEM_H
