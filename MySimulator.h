#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "House.h" 
#include "abstract_algorithm.h"
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
	void readHouseFile(std::string& houseFilePath);
    void setAlgorithm(AbstractAlgorithm& algorithm);
    House house;

private:
    AbstractAlgorithm* algorithm;
    const MyDirtSensor dirtSensor;
    const MyWallsSensor wallsSensor;
    const MyBatteryMeter batteryMeter;
    void msgLog(std::ofstream & outputFile, const std::string& msg) const;
};

#endif // SIMULATIONCONTROLLER_H