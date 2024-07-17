#ifndef MY_BATTERY_METER_H_
#define MY_BATTERY_METER_H_

#include <cstddef>
#include "battery_meter.h"
#include "House.h"


class MyBatteryMeter : public BatteryMeter {
public:
	MyBatteryMeter(House*);
	std::size_t getBatteryState() const;
private:
	House& house;
};

#endif  // MY_BATTERY_METER_H_
