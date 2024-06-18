class SensorSystem {
public:
    SensorSystem(const House& house, const VacuumCleaner& cleaner)
        : house(house), cleaner(cleaner) {}

    int checkDirt() const {
        auto pos = cleaner.getPosition();
        return house.getDirtLevel(pos.first, pos.second);
    }

    std::array<bool, 4> checkWalls() const {
        auto pos = cleaner.getPosition();
        return {house.isWall(pos.first, pos.second - 1),   // North
                house.isWall(pos.first + 1, pos.second),  // East
                house.isWall(pos.first, pos.second + 1),   // South
                house.isWall(pos.first - 1, pos.second)};  // West
    }

    int batteryStatus() const {
        // This method should be implemented based on VacuumCleaner's battery info
        return cleaner.isBatteryLow();
    }

private:
    const House& house;
    const VacuumCleaner& cleaner;
};
