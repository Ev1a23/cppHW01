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
	// Copy constructor
    MySimulator(const MySimulator& other, AbstractAlgorithm *algo)
        : house(other.house),
          algorithm(algo),
          dirtSensor(other.dirtSensor),
          wallsSensor(other.wallsSensor),
          batteryMeter(other.batteryMeter) {
    }
    void run();
	void readHouseFile(std::string& houseFilePath);
    void setAlgorithm(AbstractAlgorithm& algorithm);
    House house;
	static int calcScore(std::size_t maxSteps, std::size_t numSteps,  int dirtLeft, std::string status, bool inDock)
	{
		if (status == "DEAD")
		{
			return maxSteps + dirtLeft * 300 + 2000;
		}
		else if(status == "FINISHED" && !inDock)
		{
			return maxSteps + dirtLeft * 300 + 3000;
		}
		return numSteps + dirtLeft * 300 + (inDock ? 0 : 1000);
	}
	

private:
    AbstractAlgorithm* algorithm;
    const MyDirtSensor dirtSensor;
    const MyWallsSensor wallsSensor;
    const MyBatteryMeter batteryMeter;
    void msgLog(std::ofstream & outputFile, const std::string& msg) const;
};



#endif // SIMULATIONCONTROLLER_H