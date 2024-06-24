#ifndef SENSOR_SYSTEM_H
#define SENSOR_SYSTEM_H

#include <vector>
#include "House.h"
#include "VacuumCleaner.h"

class SensorSystem {
public:
    SensorSystem(House& house);
    std::vector<std::pair<int, int>> checkNonWalls() const;
    int batteryStatus() const;
	int checkDirt() const;
	House getHouse() const;
	VacuumCleaner getCleaner() const;

private:
    House& house;
    // const VacuumCleaner& cleaner;
};

#endif // SENSOR_SYSTEM_H
