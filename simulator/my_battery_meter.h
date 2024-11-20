#ifndef MY_BATTERY_METER_H_
#define MY_BATTERY_METER_H_

#include <cstddef>
#include "BatteryMeter.h"
#include "House.h"


class MyBatteryMeter : public BatteryMeter {

private:
	House* const house;

public:
	MyBatteryMeter(House*);
	std::size_t getBatteryState() const;

};

#endif  // MY_BATTERY_METER_H_
