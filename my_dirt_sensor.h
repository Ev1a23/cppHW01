#ifndef MY_DIRT_SENSOR_H_
#define MY_DIRT_SENSOR_H_

#include "dirt_sensor.h"
#include "House.h"

class MyDirtSensor : public DirtSensor {
public:
	MyDirtSensor(House*);
	int dirtLevel() const override = 0;

private:
	House& house;
};

#endif  // MY_DIRT_SENSOR_H_
