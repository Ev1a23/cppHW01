#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "House.h" 
#include "MyAlgorithm.h" 
#include <string>
#include <vector>
#include <fstream>
#include "my_dirt_sensor.h"
#include "my_wall_sensor.h"
#include "my_battery_meter.h"

class MySimulator {
public:
    MySimulator();
    void run();
	void msgLog(std::ofstream & outputFile, const std::string& msg);
	void readHouseFile(std::string& houseFilePath);
    void setAlgorithm(MyAlgorithm algorithm);
    House* house;

private:
    MyAlgorithm* algorithm;
};

#endif // SIMULATIONCONTROLLER_H