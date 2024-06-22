#include "Algorithm.h"

Algorithm::Algorithm(const SensorSystem& sensors)
    : sensors(sensors), dockingStation(sensors.house.getDockingStation()) {}

std::pair<int, int> Algorithm::decideNextMove()
{
    auto pos = sensors.cleaner.getPosition();
    if (pos == dockingStation && sensors.cleaner.batteryLevel() < sensors.cleaner.maxBattery())
    {
        return pos;
    }

    if (sensors.battery() == path.size() && !path.empty())
    {
        std::pair<int, int> prevStep = path.back();
        path.pop_back();
        return prevStep;
    }
    
    if (sensors.checkDirt() > 0)
    {
        if (sensors.checkDirt() == 1)
        {
            cleanedLocations.insert(pos);
        }
        return pos;
    }

    cleanedLocations.insert(pos);
    std::vector<std::pair<int, int>> nonWallLocations = sensors.checkNonWalls();
    int minDirtLevel = 100;
    std::pair<int, int> nextMove = pos;
    for (auto& location : nonWallLocations)
    {
        int dirtLevel = sensors.house.getDirtLevel(location.first, location.second);
        if (cleanedLocations.find(location) == cleanedLocations.end() && dirtLevel > 0 && dirtLevel < minDirtLevel)
        {
            nextMove = location;
            minDirtLevel = dirtLevel;
        }
        if (dirtLevel == 0)
        {
            cleanedLocations.insert(location);
        }
    }
    path.push_back(nextMove);
    return nextMove;
}
