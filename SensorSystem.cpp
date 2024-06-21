class SensorSystem {
public:
    SensorSystem(const House& house, const VacuumCleaner& cleaner)
        : house(house), cleaner(cleaner) {}

    int checkDirt() const {
        auto pos = cleaner.getPosition();
        return house.getDirtLevel(pos.first, pos.second);
    }

    std::vector<std::pair<int, int>> checkNonWalls() const {
        auto pos = cleaner.getPosition();
        std::vector<std::pair<int, int>> nonWallLocations;

        // North
        if (!house.isWall(pos.first, pos.second - 1)) {
            nonWallLocations.push_back({pos.first, pos.second - 1});
        }

        // East
        if (!house.isWall(pos.first + 1, pos.second)) {
            nonWallLocations.push_back({pos.first + 1, pos.second});
        }

        // South
        if (!house.isWall(pos.first, pos.second + 1)) {
            nonWallLocations.push_back({pos.first, pos.second + 1});
        }

        // West
        if (!house.isWall(pos.first - 1, pos.second)) {
            nonWallLocations.push_back({pos.first - 1, pos.second});
        }

        return nonWallLocations;
    }

    int batteryStatus() const {
        // This method should be implemented based on VacuumCleaner's battery info
        return cleaner.isBatteryLow();
    }

private:
    const House& house;
    const VacuumCleaner& cleaner;
};
