#ifndef MY_DIRT_SENSOR_H_
#define MY_DIRT_SENSOR_H_

class MyDirtSensor : public my_dirt_sensor {
public:
	MyDirtSensor(House&);
	int dirtLevel() const = 0;

private:
	House& house;
};

#endif  // MY_DIRT_SENSOR_H_
