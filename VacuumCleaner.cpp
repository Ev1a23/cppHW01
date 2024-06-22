#include "VacuumCleaner.h"
class VacuumCleaner {
public:
    VacuumCleaner(int maxBattery, const std::pair<int, int>& startPosition)
        : battery(maxBattery), maxBattery(maxBattery), position(startPosition), dockedSteps(0) {}

    void move(int x, int y) {
        if (x != position.first || y != position.second) {  // Moving to a new position
            position = {x, y};
            dockedSteps = 0;  // Reset docked steps if moving
        }
    }

    void clean() {
        // Assuming that when the cleaner cleans, it decreases the dirt level by 1.
        // This method is expected to be called when there is dirt (not handled here).
    }

    void charge() {
        // Increment dockedSteps each time this function is called while docked
        dockedSteps++;
        double rechargeAmount = static_cast<double>(maxBattery) / 20.0;
        battery += static_cast<int>(std::round(rechargeAmount));  // Use std::round to round to the nearest int
        if (battery > maxBattery) {
            battery = maxBattery;  // Cap the battery at its maximum capacity
        }
    }

    int batteryLevel()
    {
        return battery;
    }
    
    std::pair<int, int> getPosition() const {
        return position;
    }

private:
    int battery;  // Holds the current battery level
    int maxBattery;  // The maximum battery capacity
    std::pair<int, int> position;  // The current position of the vacuum cleaner
    int dockedSteps;  // Tracks the number of steps since docking
};
