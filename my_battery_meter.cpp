#include "my_battery_meter.h"

MyBatteryMeter::MyBatteryMeter(House* house) : house(*house) {}
std::size_t MyBatteryMeter::getBatteryState() const
{
	return house.getCleaner().batteryLevel();
}