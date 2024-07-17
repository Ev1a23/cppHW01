#ifndef MY_WALL_SENSOR_H_
#define MY_WALL_SENSOR_H_

// #include "enums.h"
#include "wall_sensor.h"
#include "House.h"

class MyWallsSensor : public WallsSensor {
public:
	MyWallsSensor(House*);
	bool isWall(Direction d) const;

private:
	House& house;
};

#endif  // MY_WALL_SENSOR_H_
