#include "my_battery_meter.h"
#include <iostream>


MyBatteryMeter::MyBatteryMeter(House* house) : house(*house) {}
std::size_t MyBatteryMeter::getBatteryState() const
{
	std::cout << "Im in MyBatteryMeter::getBatteryState()\n";
	return house.getCleaner().batteryLevel();
}