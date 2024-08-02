#include "my_dirt_sensor.h"
#include <iostream>

MyDirtSensor::MyDirtSensor(House* house) : house(house) {}
int MyDirtSensor::dirtLevel() const
{
    std::pair<int,int> pos = house->getCleaner().getPosition();
    house->getGrid()[pos.first][pos.second];

    std::cout << "CCCCCC\n";
    return house->getGrid()[pos.first][pos.second];
}