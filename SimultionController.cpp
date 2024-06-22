#include <iostream>
class SimulationController {
public:
    SimulationController(std::string inputFilePath, std::string outputFilePath)
    {
        this->inputFilePath = inputFilePath;
        this->outputFilePath = outputFilePath;
        this->house = FileManagement::loadHouse(inputFilePath);
        this->cleaner = VacuumCleaner(10, dockingStation);
        this->sensors = SensorSystem(house, cleaner);
        this->algorithm = Algorithm(sensors);
        }
    void runSimulation()
    {
        int cnt = 0;
        while(house.getDirtLevel() > 0 && house.getMaxAllowedSteps() > cnt)
        {
            cnt++;
            pair<int, int> curPos = cleaner.getPosition();
            std::pair<int, int> nextMove = algorithm.decideNextMove();
            std::cout << "Current Position: (" << curPos.first << ", " << curPos.second << "), Next Move: (" << nextMove.first << ", " << nextMove.second << ")\n";
            if(nextMove == house.getDockingStation())
            {
                if(nextMove != curPos)
                {
                    std:cout << "Moving to docking station\n";
                    cleaner.move(nextMove);
                }
                else
                {
                    std:cout << "Charging at docking station\n";
                    cleaner.charge();
                }
            }
            else
            {
                if(nextMove != curPos)
                {
                    std:cout << "Moving to next location\n";
                    cleaner.move(nextMove);
                }
                else
                {
                    std:cout << "Cleaning at current location\n";
                    house.cleanPos(nextMove.first, nextMove.second);
                }
            }
            
        }
    }
private:
    House house;
    VacuumCleaner cleaner;
    SensorSystem sensors;
    Algorithm algorithm;
    std::vector<std::string> log;
};