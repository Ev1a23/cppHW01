int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return 1;
    }
    std::string inputFilePath = argv[1];
    std::string outputFilePath = "output.txt";

    SimulationController simController(inputFilePath, outputFilePath);
    simController.runSimulation();
    return 0;
}
