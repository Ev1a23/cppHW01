class SimulationController {
public:
    SimulationController(std::string inputFilePath, std::string outputFilePath);
    void runSimulation();
private:
    House house;
    VacuumCleaner cleaner;
    SensorSystem sensors;
    Algorithm algorithm;
    std::vector<std::string> log;
};