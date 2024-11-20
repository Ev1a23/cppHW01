#ifndef MY_WALL_SENSOR_H_
#define MY_WALL_SENSOR_H_

// #include "enums.h"
#include "WallSensor.h"
#include "House.h"

class MyWallsSensor : public WallsSensor {
private:
	const House* house;

public:
	MyWallsSensor(House*);
	bool isWall(Direction d) const;

};

#endif  // MY_WALL_SENSOR_H_
