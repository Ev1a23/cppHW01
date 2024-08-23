#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "House.h" 
#include "common/AbstractAlgorithm.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "my_dirt_sensor.h"
#include "my_wall_sensor.h"
#include "my_battery_meter.h"


class MySimulator {
public:
    MySimulator();
    void run();
	// void readHouseFile(std::string& houseFilePath);
    void setAlgorithm(AbstractAlgorithm& algorithm);
	void setHouse(House h) {house = h;}
	// std::string getSummaryString() {return summary.str();}
	int calcScore(std::size_t maxSteps, std::size_t numSteps,  int dirtLeft, std::string status, bool inDock);
	int getScore() {return results.score;}
	struct SimResults {
		size_t numSteps;
		int dirtLeft;
		std::string status;
		std::string inDock;
		int score;
		std::string stepsLog;

		void setValues(size_t numSteps_, int dirtLeft_, std::string status_, std::string inDock_, int score_, std::string stepsLog_) {
			numSteps = numSteps_;
			dirtLeft = dirtLeft_;
			status = status_;
			inDock = inDock_;
			score = score_;
			stepsLog = stepsLog_;
		}

		std::string str(){
			std::ostringstream logStream;
			logStream << "NumSteps = " << numSteps << "\nDirtLeft = " << dirtLeft << "\nStatus = " << status << "\nInDock = " << inDock << "\nScore = " << score <<"\nSteps:\n" << stepsLog;
			return logStream.str();
		}
	};
	SimResults getResults() {return results;}
	

private:
    AbstractAlgorithm* algorithm;
    const MyDirtSensor dirtSensor;
    const MyWallsSensor wallsSensor;
    const MyBatteryMeter batteryMeter;
	House house;
	SimResults results;
	std::string outputFile;
    void msgLog(std::ofstream & outputFile, const std::string& msg) const;
};



#endif // SIMULATIONCONTROLLER_H