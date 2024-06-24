#include "VacuumCleaner.h"
VacuumCleaner::VacuumCleaner(int maxBattery, const std::pair<int, int>& startPosition)
        : battery(maxBattery), maxBattery(maxBattery), position(startPosition), dockedSteps(0) {}
 
void VacuumCleaner::move(int x, int y) {
        if (x != position.first || y != position.second) {  // Moving to a new position
            position = {x, y};
            dockedSteps = 0;  // Reset docked steps if moving
        }
		battery--;  // Moving consumes battery
    }
 
void VacuumCleaner::clean() {
        // Assuming that when the cleaner cleans, it decreases the dirt level by 1.
        // This method is expected to be called when there is dirt (not handled here).
    }
 
void VacuumCleaner::charge() {
        // Increment dockedSteps each time this function is called while docked
        dockedSteps++;
        double rechargeAmount = static_cast<double>(maxBattery) / 20.0;
        battery += static_cast<int>(std::round(rechargeAmount));  // Use std::round to round to the nearest int
        if (battery > maxBattery) {
            battery = maxBattery;  // Cap the battery at its maximum capacity
        }
    }
 
int VacuumCleaner::batteryLevel() const
    {
        return battery;
    }

int VacuumCleaner::maxBatteryLevel() const
	{
		return maxBattery;
	}
    
std::pair<int, int> VacuumCleaner::getPosition() const {
        return position;
    }
 