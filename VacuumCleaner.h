#ifndef VACUUMCLEANER_H
#define VACUUMCLEANER_H

#include <utility>
#include <cmath>

class VacuumCleaner {
public:
    VacuumCleaner(double maxBattery, const std::pair<int, int> startPosition);
    VacuumCleaner();

    void move(int x, int y);
    void clean();
    void charge();
    double batteryLevel() const;
	double maxBatteryLevel() const;
    std::pair<int, int> getPosition() const;

private:
    double battery;
    double maxBattery;
    std::pair<int, int> position;
    // int dockedSteps;
};

#endif // VACUUMCLEANER_H