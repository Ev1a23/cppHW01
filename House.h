#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <vector>
#include <utility>

class House {

public:
	std::pair<int,int> moveTranslation(int x);
    class VacuumCleaner {
    public:
        void move(int x, int y);
        void clean();
        void charge();
        std::size_t batteryLevel() const; // should be accessed by SensorSystem so probably has to be public, 
                                        // anyway no one can have an instance of VacuumCleaner besides within House classes
        std::pair<int, int> getPosition() const; // should be private so only sensor function can access it
        double getMaxBatterySteps() const;
        std::size_t getMaxAllowedSteps() const;
        VacuumCleaner(const std::size_t maxAllowedSteps, const std::size_t maxBattery, const std::pair<int, int> startPosition);
        // VacuumCleaner();

    private:
        // VacuumCleaner(const int maxAllowedSteps, const double maxBattery, const std::pair<int, int> startPosition);
        // VacuumCleaner();
        size_t maxAllowedSteps;
        std::size_t battery;
        std::size_t maxBattery;
        std::pair<int, int> position;
    };

    class SensorSystem {
    public:
        SensorSystem(House& house); // access from House only
        // SensorSystem(); // access from House only
        std::vector<std::pair<int, int>> wallsSensor() const; // returns a vector of the non-walls neighbors
        double  batterySensor() const;
        int dirtSensor() const;
        // House getHouse() const;
        // VacuumCleaner getCleaner() const;

    private:
        House& house;
        // SensorSystem(); // access from House only
    };

    House(const std::string& inputFile);
    // int getDirtLevel(int x, int y) const;
    std::pair<int, int> getDockingStation() const;
    int getTotalDirt() const;
    // VacuumCleaner& getCleaner(); no need
    SensorSystem& getSensors();
    VacuumCleaner& getCleaner();
    void clean();

private:
    std::vector<std::vector<int>> grid;
    std::pair<int, int> dockingStation = std::make_pair(-1, -1);
    int totalDirt;
    void loadHouse(const std::string& path);
    bool isWall(int x, int y) const; // priviously public

    VacuumCleaner cleaner;
    SensorSystem sensors;

};

#endif