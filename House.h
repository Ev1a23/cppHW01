#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <vector>
#include <utility>
#include "VacuumCleaner.h"

class House {
public:
    House(const std::string& inputFile);
    // int getDirtLevel(int x, int y) const;
    std::pair<int, int> getDockingStation() const;
    int getTotalDirt() const;
    double getMaxBatterySteps() const;
    int getMaxAllowedSteps() const;
    // VacuumCleaner& getCleaner(); no need
    SensorSystem& getSensors();

private:
    std::vector<std::vector<int>> grid;
    std::pair<int, int> dockingStation = std::make_pair(-1, -1);
    int totalDirt;
    // int maxAllowedSteps; // should be at VacuumCleaner
    VacuumCleaner cleaner;
    SensorSystem sensors;
    void loadHouse(const std::string& path);
    bool isWall(int x, int y) const; // priviously public
    void cleanPos(int x, int y); 
    class SensorSystem {
    public:
        std::vector<std::pair<int, int>> wallsSensor() const; // returns a vector of the non-walls neighbors
        double  batterySensor() const;
        int dirtSensor() const;
        // House getHouse() const;
        // VacuumCleaner getCleaner() const;

    private:
        SensorSystem(); // access from House only
    };

    class VacuumCleaner {
    public:
        void move(int x, int y);
        void clean();
        void charge();
        double batteryLevel() const; // should be accessed by SensorSystem so probably has to be public, 
                                        // anyway no one can have an instance of VacuumCleaner besides within House classes
        // double maxBatteryLevel() const; // shouldnt be accessable to everyone
        // std::pair<int, int> getPosition() const; // should be private so only sensor function can access it

    private:
        VacuumCleaner(double maxBattery, const std::pair<int, int> startPosition);
        VacuumCleaner();
        double battery;
        double maxBattery;
        std::pair<int, int> position;
        // int dockedSteps;
        std::pair<int, int> getPosition() const;
    };
}