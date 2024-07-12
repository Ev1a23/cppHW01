#include "my_battery_meter.h"

MyBatteryMeter::MyBatteryMeter(House& house) : house(house) {}
std::size_t getBatteryState() const
{
	return house.getCleaner().batteryLevel();
}