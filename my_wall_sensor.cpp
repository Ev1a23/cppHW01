#include "my_wall_sensor.h"

MyWallsSensor::MyWallsSensor(House& house) : house(house) {}

bool isWall(Direction d) const
{
	std::pair<int,int> check_me = house.moveTranslation(d);
	return (house.isWall(check_me.first, check_me.second));
}
