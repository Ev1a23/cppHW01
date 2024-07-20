#include "my_dirt_sensor.h"
#include <iostream>

MyDirtSensor::MyDirtSensor(House* house) : house(house) {}
int MyDirtSensor::dirtLevel() const
{
    // std::cout << "Im in MyDirtSensor::dirtLevel()" << std::endl;
    std::pair<int,int> pos = house->getCleaner().getPosition();
    // std::cout << pos.first << "," << pos.second << "\n";
    return house->grid[pos.first][pos.second]; 
}