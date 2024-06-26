#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <vector>
#include <utility>
#include "VacuumCleaner.h"

class House {
public:
    House(const std::string& inputFile);
    int getDirtLevel(int x, int y) const;
    bool isWall(int x, int y) const;
    std::pair<int, int> getDockingStation() const;
    int getTotalDirt() const;
    double getMaxBatterySteps() const;
    int getMaxAllowedSteps() const;
    void cleanPos(int x, int y);
    VacuumCleaner& getCleaner();

private:
    std::vector<std::vector<int>> grid;
    std::pair<int, int> dockingStation = std::make_pair(-1, -1);
    int totalDirt;
    int maxAllowedSteps;
    VacuumCleaner cleaner;
    void loadHouse(const std::string& path);};

#endif // HOUSE_H