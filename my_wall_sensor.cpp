#include "my_wall_sensor.h"
#include <iostream>

MyWallsSensor::MyWallsSensor(House* house) : house(house) {}

bool MyWallsSensor::isWall(Direction d) const
{
	std::pair<int,int> check_me = house->moveTranslation(static_cast<Step>(d));
	//std::cout << "MyWallsSensor::isWall\n--------------\nTranslated Move! " << check_me.first << ","  << check_me.second << "\n";
	return (house->isWall(check_me.first, check_me.second));
}
