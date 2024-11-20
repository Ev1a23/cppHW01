#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <vector>
#include <utility>
#include <filesystem>
#include "enums.h"

#include <iostream>

namespace fs = std::filesystem;

class House {

public:

    class VacuumCleaner {
    public:
        VacuumCleaner(const std::size_t maxAllowedSteps, const double maxBattery, const std::pair<int, int> startPosition);
        VacuumCleaner();

        std::pair<int, int> getPosition() const;
        std::size_t getMaxBatterySteps() const;
        std::size_t getMaxAllowedSteps() const;

        void move(int x, int y);
        void clean();
        void charge();
        double batteryLevel() const; // should be accessed by SensorSystem so probably has to be public, 
                                     // anyway no one can have an instance of VacuumCleaner besides within House classes
        
    private:
        std::size_t maxAllowedSteps;
        double battery;
        double maxBattery;
        std::pair<int, int> position;
    };

    House();
    House(const fs::path inputFile);

    std::pair<int, int> getDockingStation() const;
    VacuumCleaner& getCleaner();
    std::vector<std::vector<int>> getGrid() const;

	std::pair<int,int> moveTranslation(Step x) const;
    bool isWall(int x, int y) const;
    void clean();
    int totalDirt();
    std::string fileName() {return std::string(file.filename().replace_extension("").string());};
    std::size_t getMaxSteps() const {return cleaner.getMaxAllowedSteps();};

private:
    std::vector<std::vector<int>> grid;
    std::pair<int, int> dockingStation = std::make_pair(-1, -1);
    VacuumCleaner cleaner;
    fs::path file;
    void loadHouse(const fs::path& file);
};

#endif