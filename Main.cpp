#include "MySimulator.h"
#include <iostream>
#include "MyAlgorithm.h"

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
		// int x = simulator.house->getCleaner().getPosition().first;
		// std::cout << "x = " << x << "\n";
		std::cout << "first = " << simulator.house.getCleaner().getPosition().first << "\n";
		std::cout << "first = " << simulator.house.getCleaner().getPosition().first << "\n";
		std::cout << "first = " << simulator.house.getCleaner().getPosition().first << "\n";

		MyAlgorithm algo;
		//std::cout << "first = " << simulator.house->getCleaner().getPosition().first << "\n";
		simulator.setAlgorithm(algo);
		//std::cout << "first = " << simulator.house->getCleaner().getPosition().first << "\n";
		simulator.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
    return 0;
}
