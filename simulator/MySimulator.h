#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "House.h" 
#include "abstract_algorithm.h"
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
    int run();
	void readHouseFile(std::string& houseFilePath);
    void setAlgorithm(AbstractAlgorithm& algorithm);
	std::string getSummaryString() {return summary.str();}
    House house;
	int calcScore(std::size_t maxSteps, std::size_t numSteps,  int dirtLeft, std::string status, bool inDock)
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
	int getScore() {return summary.score;}
	struct SimResults
	{
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


	

private:
    AbstractAlgorithm* algorithm;
    const MyDirtSensor dirtSensor;
    const MyWallsSensor wallsSensor;
    const MyBatteryMeter batteryMeter;
	SimResults summary = {};
    void msgLog(std::ofstream & outputFile, const std::string& msg) const;
};



#endif // SIMULATIONCONTROLLER_H