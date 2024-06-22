#ifndef VACUUMCLEANER_H
#define VACUUMCLEANER_H

#include <utility>
#include <cmath>

class VacuumCleaner {
public:
    VacuumCleaner(int maxBattery, const std::pair<int, int>& startPosition);

    void move(int x, int y);
    void clean();
    void charge();
    int batteryLevel() const;
    std::pair<int, int> getPosition() const;

private:
    int battery;
    int maxBattery;
    std::pair<int, int> position;
    int dockedSteps;
};

#endif // VACUUMCLEANER_H