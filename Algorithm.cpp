#include <set>
class Algorithm {
public:
    Algorithm(const SensorSystem& sensors);
    std::pair<int,int> decideNextMove()
    {
        auto pos = sensors.cleaner.getPosition();
        if(pos == dockingStation && sensors.cleaner.batteryLevel() < sensors.cleaner.maxBattery)
        {
            return pos;
        }

        if(sensors.battery == path.size() && !path.empty())
        {
            std::pair<int, int> prevStep = path.back();
            path.pop_back();
            return prevStep;
        }
        
        if(sensors.checkDirt() > 0)
        {
            if(sensors.checkDirt() == 1)
            {
                cleanedLocations.insert(pos);
            }
            return pos;
        }
        
        cleanedLocations.insert(pos);
        std::vector<std::pair<int, int>> nonWallLocations = sensors.checkNonWalls();
        int minDirtLevel = 100;
        ste:pair<int, int> nextMove = pos;
        for(auto location : nonWallLocations)
        {
            if(cleanedLocations.find(location) == cleanedLocations.end() && sensors.house.getDirtLevel(location.first, location.second) > 0 && sensors.house.getDirtLevel(location.first, location.second) < minDirtLevel)
            {
                nextMove = location;
            }
            if(sensors.house.getDirtLevel(location.first, location.second) == 0)
            {
                cleanedLocations.insert(location);
            }
        }
        path.push_back(nextMove);
        return nextMove;
    }
private:
    const SensorSystem& sensors;
    std::vector<std::pair<int, int>> path;
    std::set<std::pair<int, int>> cleanedLocations;
    const std:pair<int, int> dockingStation = sensors.house.getDocking();
};
