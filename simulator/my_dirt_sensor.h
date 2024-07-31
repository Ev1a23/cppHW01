#ifndef MY_DIRT_SENSOR_H_
#define MY_DIRT_SENSOR_H_

#include "dirt_sensor.h"
#include "House.h"

class MyDirtSensor : public DirtSensor {
private:
	House* const house;

public:
	MyDirtSensor(House*);
	int dirtLevel() const;

};

#endif  // MY_DIRT_SENSOR_H_
