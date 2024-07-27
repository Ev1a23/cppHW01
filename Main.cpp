#include "MySimulator.h"
#include <iostream>
#include "MyAlgorithm.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

// Structure to hold the parsed options
struct Options {
    std::string house_path;
    std::string algo_path;
    int num_threads;
    bool summary_only;
};

// Function to parse command-line arguments
bool parseInput(int argc, char* argv[], Options& options) {
    namespace po = boost::program_options;

    // Default values
    options.house_path = ".";
    options.algo_path = ".";
    options.num_threads = 10;
    options.summary_only = false;

    // Define the options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("house_path", po::value<std::string>(&options.house_path)->default_value("."), "path to the house file")
        ("algo_path", po::value<std::string>(&options.algo_path)->default_value("."), "path to the algorithm file")
        ("num_threads", po::value<int>(&options.num_threads)->default_value(10), "number of threads")
        ("summary_only", po::bool_switch(&options.summary_only), "output summary only");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return false;
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }

    return true;
}


int main(int argc, char* argv[]) {
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
    //     return 1;
    // }
    // std::string houseFilePath = argv[1];
    Options options;

    if (!parseInput(argc, argv, options)) {
        return 1;
    }

    // Output the values
    std::cout << "House path: " << options.house_path << "\n";
    std::cout << "Algorithm path: " << options.algo_path << "\n";
    std::cout << "Number of threads: " << options.num_threads << "\n";
    std::cout << "Summary only: " << std::boolalpha << options.summary_only << "\n";

    // generate the cartesian product of algos x houses:
        // 1. house file path
        // 2. algorithm instance of some form (.so file)
        // 3. simulator to run this algo on this file

    // generate <num_threads> threads
    // send each pair (its simulator.run()) to a thread
    // when a thread ends, use ot again or replace it and send it the next pair, until all pairs were processed
    // 


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
