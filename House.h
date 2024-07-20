#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <vector>
#include <utility>
#include "enums.h"

class House {

public:
	std::pair<int,int> moveTranslation(Step x);
    bool isWall(int x, int y) const;
    std::vector<std::vector<int>> grid;
    class VacuumCleaner {
    public:
        void move(int x, int y);
        void clean();
        void charge();
        double batteryLevel() const; // should be accessed by SensorSystem so probably has to be public, 
                                     // anyway no one can have an instance of VacuumCleaner besides within House classes
        std::pair<int, int> getPosition() const;
        std::size_t getMaxBatterySteps() const;
        std::size_t getMaxAllowedSteps() const;
        VacuumCleaner(const std::size_t maxAllowedSteps, double maxBattery, const std::pair<int, int> startPosition);
        VacuumCleaner();

    private:
        std::size_t maxAllowedSteps;
        double battery;
        double maxBattery;
        std::pair<int, int> position;
    };
    House();
    House(const std::string& inputFile);
    std::pair<int, int> getDockingStation() const;
    VacuumCleaner& getCleaner();
    void clean();

private:
    std::pair<int, int> dockingStation = std::make_pair(-1, -1);
    int totalDirt;
    void loadHouse(const std::string& path);
    VacuumCleaner cleaner;
};

#endif