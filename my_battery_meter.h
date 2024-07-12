#ifndef MY_BATTERY_METER_H_
#define MY_BATTERY_METER_H_

#include <cstddef>

class MyBatteryMeter : public BatteryMeter {
public:
	std::size_t getBatteryState() const = 0;
private:
	House& house;
};

#endif  // MY_BATTERY_METER_H_
