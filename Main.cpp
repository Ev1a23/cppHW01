#include "SimulationController.h"
#include <iostream>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return 1;
    }
    std::string inputFilePath = argv[1];
	try
	{
    	SimulationController simController(inputFilePath);
    	simController.runSimulation();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
    return 0;
}
