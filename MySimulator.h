#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "House.h" 
#include "Algorithm.h" 
#include <string>
#include <vector>
#include <fstream>

class MySimulator {
public:
    MySimulator();
    void run();
	void msgLog(std::ofstream & outputFile, const std::string& msg);
	void readHouseFile(std::string& houseFilePath);

private:
	House* house;
    Algorithm* algorithm;
};

#endif // SIMULATIONCONTROLLER_H