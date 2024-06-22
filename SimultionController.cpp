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
        pair<int, int> curPos = cleaner.getPosition();
        while(house.getDirtLevel() > 0 && house.getMaxAllowedSteps() > cnt)
        {
            cnt++;
            std::pair<int, int> nextMove = algorithm.decideNextMove();
            if(nextMove == house.getDockingStation())
            {
                if(nextMove != curPos)
                {
                    cleaner.move(nextMove);
                }
                else
                {
                    cleaner.charge();
                }
            }
            else
            {
                if(nextMove != curPos)
                {
                    cleaner.move(nextMove);
                }
                else
                {
                    cleaner.clean();
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