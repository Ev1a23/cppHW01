#include "MySimulator.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return 1;
    }
    std::string houseFilePath = argv[1];
	try
	{
    	MySimulator simulator;
		simulator.readHouseFile(houseFilePath);
		MyAlgorithm algo;
		simulator.setAlgorithm(algo);
		simulator.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
    return 0;
}
