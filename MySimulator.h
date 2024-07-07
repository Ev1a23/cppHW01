#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "House.h" 
#include "Algorithm.h" 
#include <string>
#include <vector>
#include <fstream>

class MySimulator {
public:
    MySimulator(std::string inputFilePath);
    // void readHouseFile(std::string& houseFilePath);
    void run();
	void msgLog(std::ofstream & outputFile, const std::string& msg);

private:
    std::string inputFilePath;
	House house;
    Algorithm algorithm;
};

#endif // SIMULATIONCONTROLLER_H