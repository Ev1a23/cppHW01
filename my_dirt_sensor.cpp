#include "my_dirt_sensor.h"

MyDirtSensor::MyDirtSensor(House* house) : house(*house) {}
int MyDirtSensor::dirtLevel() const
{
	std::pair<int,int> pos = house.getCleaner().getPosition();
    return house.grid[pos.first][pos.second]; 
}