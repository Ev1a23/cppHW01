#include "MySimulator.h"
#include <iostream>

// int main(int argc, char** argv) {
// 	MySimulator::MySimulator simulator;
// 	// TODO: get houseFilePath from command line
// 	simulator.readHouseFile(houseFilePath);
// 	MySimulator::MyAlgorithm algo;
// 	simulator.setAlgorithm(algo);
// 	simulator.run();
// }

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return 1;
    }
    std::string inputFilePath = argv[1];
	try
	{
    	MySimulator simController(inputFilePath);
    	simController.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
    return 0;
}
