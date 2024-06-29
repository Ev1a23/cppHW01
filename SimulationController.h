#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "House.h" 
#include "Algorithm.h" 
#include <string>
#include <vector>
#include <fstream>

class SimulationController {
public:
    SimulationController(const std::string& inputFilePath);
    void runSimulation();
	void msgLog(std::ofstream & outputFile, const std::string& msg);

private:
    std::string inputFilePath;
	House house;
    Algorithm algorithm;
};

#endif // SIMULATIONCONTROLLER_H