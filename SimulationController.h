#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "FileManagement.h" 
#include "House.h" 
#include "VacuumCleaner.h" 
#include "SensorSystem.h" 
#include "Algorithm.h" 
#include <string>
#include <vector>

class SimulationController {
public:
    SimulationController(const std::string& inputFilePath, const std::string& outputFilePath);
    void runSimulation();

private:
    House house;
    VacuumCleaner cleaner;
    SensorSystem sensors;
    Algorithm algorithm;
    std::vector<std::string> log;
    std::string inputFilePath;
    std::string outputFilePath;
};

#endif // SIMULATIONCONTROLLER_H