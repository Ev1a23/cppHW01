#include "SensorSystem.h"

SensorSystem::SensorSystem(const House& house, const VacuumCleaner& cleaner)
    : house(house), cleaner(cleaner) {}

int SensorSystem::checkDirt() const {
    auto pos = cleaner.getPosition();
    return house.getDirtLevel(pos.first, pos.second);
}

std::vector<std::pair<int, int>> SensorSystem::checkNonWalls() const {
    auto pos = cleaner.getPosition();
    std::vector<std::pair<int, int>> nonWallLocations;

    // Check each direction for non-wall positions
    if (!house.isWall(pos.first, pos.second - 1)) {
        nonWallLocations.push_back({pos.first, pos.second - 1});  // North
    }
    if (!house.isWall(pos.first + 1, pos.second)) {
        nonWallLocations.push_back({pos.first + 1, pos.second});  // East
    }
    if (!house.isWall(pos.first, pos.second + 1)) {
        nonWallLocations.push_back({pos.first, pos.second + 1});  // South
    }
    if (!house.isWall(pos.first - 1, pos.second)) {
        nonWallLocations.push_back({pos.first - 1, pos.second});  // West
    }

    return nonWallLocations;
}

int SensorSystem::batteryStatus() const {
    // Assuming 'isBatteryLow()' method exists and returns an integer battery status
    // Modify the method according to your VacuumCleaner's implementation
    return cleaner.batteryLevel();
}
