#include "AlgorithmRegistrar.h"
#include "MySimulator.h"
#include <iostream>
// #include "MyAlgorithm.h"
// #include "RandomizedStepAlgorithm.h"
// #include "BFSAlgorithm.h"

//#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <cstring>

#include <iostream>
#include <string>
#include <filesystem>

#include <dlfcn.h>

namespace fs = std::filesystem;

void handleInvalidFileException(const std::exception &e, const fs::path &invalidFilePath)
{
    fs::path errorPath = invalidFilePath;
    errorPath = fs::current_path().string() / errorPath.replace_extension("error").filename();
    std::ofstream errorFile(errorPath);
    if (!errorFile) {
        std::cerr << "Failed to open file: " << invalidFilePath << std::endl;
        throw std::runtime_error("Could not write error file");
    }
    errorFile <<  e.what() << std::endl;
}

bool validateHouseFile(MySimulator &sim, const fs::path &houseFilePath)
{
    try
    {
        std::string houseFilePath_str = houseFilePath.string();
        sim.readHouseFile(houseFilePath_str);
    }
    catch (const std::exception& e)
    {
        handleInvalidFileException(e, houseFilePath);
        return false;
    }
    return true;
}

bool validateAlgoFile(const fs::path &algoFilePath)
{
    size_t algoCountBefore = AlgorithmRegistrar::getAlgorithmRegistrar().count(); // how many algorithm are registered so far
    void *algoFilePtr;
    try
    {
        algoFilePtr = dlopen(algoFilePath.string().c_str(), RTLD_LAZY);
        if (!(algoFilePtr))
        {
            throw std::runtime_error("dlopen failed openning " + algoFilePath.string());
        }
        else if (algoCountBefore == AlgorithmRegistrar::getAlgorithmRegistrar().count())
        {
            throw std::runtime_error("Algorithm " + algoFilePath.string() + " failed to regiter, make sure you call REGISTER_ALGORITHM("+algoFilePath.string()+") in it.");
        }
    }
    catch (const std::exception& e)
    {
        handleInvalidFileException(e, algoFilePath);
        return false;
    }
    return true;
}

struct Config {
    std::string house_path = fs::current_path().string();
    std::string algo_path = fs::current_path().string();
    int num_threads = 10;
    bool summary_only = false;
};

Config parse_args(int argc, char* argv[]) {
    Config config;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("-house_path=") == 0) {
            config.house_path = arg.substr(strlen("-house_path="));
        } else if (arg.find("-algo_path=") == 0) {
            config.algo_path = arg.substr(strlen("-algo_path="));
        } else if (arg.find("-num_threads=") == 0) {
            config.num_threads = std::stoi(arg.substr(strlen("-num_threads=")));
        } else if (arg == "-summary_only") {
            config.summary_only = true;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
        }
    }

    return config;
}

void process_houses(const std::string& house_path, std::vector<std::vector<MySimulator>> &simulators)
{
    for (const auto& house_entry : fs::directory_iterator(house_path)) 
    {
        if (house_entry.is_regular_file() && house_entry.path().extension() == ".house") 
        {
            MySimulator sim;
            simulators.push_back(sim);
            if (!validateHouseFile(simulators.back(), house_entry.path()))
            {
                simulators.pop_back();
            }
        }
    }
}

void process_algos(const std::string& algo_path) 
{
    for (const auto& algo_entry : fs::directory_iterator(algo_path)) 
    {
        if (algo_entry.is_regular_file() && algo_entry.path().extension() == ".so") {
            validateAlgoFile(algo_entry.path);
        }
    }
}

int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
//         return 1;
//     }
//      std::string houseFilePath = argv[1];
    // Options options;

    // if (!parseInput(argc, argv, options)) {
    //     return 1;
    // }
    // std::string houseFilePath = argv[1];
    try
    {
        Config config = parse_args(argc, argv);

        std::cout << "House Path: " << config.house_path << std::endl;
        std::cout << "Algo Path: " << config.algo_path << std::endl;
        std::cout << "Number of Threads: " << config.num_threads << std::endl;
        std::cout << "Summary Only: " << (config.summary_only ? "true" : "false") << std::endl;

        std::vector<MySimulator> simulators;
        std::vector<std::unique_ptr<AbstractAlgorithm>> algorithms;  // this is here just to make the algorithms local of main
                                                    // so they will be available to simualtors the whole run
        process_houses(config.house_path, simulators);
        process_algos(config.algo_path);

        // simple running with no concurrent simulations:
        for (int i = 0; i < simulators.size(); i++)
        {
            for (algo : AlgorithmRegistrar::getAlgorithmRegistrar())
            {
                try
                {
                    algorithms.push_back(algo->create());
                    if (algorithms.back() == NULL)
                    {
                        throw std::runtime_error("algorithm is not valid: " + algo->name()); 
                    }
                    std::cout << "Setting algorithm ti simulation: " << algo->name() << std::endl;
                    simulators[i].setAlgorithm(*algorithms.back());
                }
                catch (const std::exception& e)
                {
                    handleInvalidFileException(e, fs::path(algoName));
                }
            }
            std::string algoName = (AlgorithmRegistrar::getAlgorithmRegistrar().begin() + i)->name();
            try
            {
                algorithms.push_back((AlgorithmRegistrar::getAlgorithmRegistrar().begin() + i)->create());
                if (algorithms.back() == NULL)
                {
                    throw std::runtime_error("algorithm is not valid: " + algoName); 
                }
                std::cout << "Setting algorithm ti simulation: " << algoName << std::endl;
                simulators[i].setAlgorithm(*algorithms.back());
            }
            catch (const std::exception& e)
            {
                handleInvalidFileException(e, fs::path(algoName));
            }
        }

        for (MySimulator sim : simulators)
        {
            std::cout << "house addr in main: " << &(sim.house) << std::endl;
            sim.run();
        }


        // generate the cartesian product of algos x houses:
            // list of all house files
            // list of all .so files
            // 1. house file path
            // 2. algorithm instance of some form (.so file)
            // 3. simulator to run this algo on this file

        // generate <num_threads> threads
        // send each pair (its simulator.run()) to a thread
        // when a thread ends, use ot again or replace it and send it the next pair, until all pairs were processed
        // 


        // try
        // {
        // 	MySimulator simulator;
        // 	simulator.readHouseFile(houseFilePath);
        // 	MyAlgorithm algo;
        // 	simulator.setAlgorithm(algo);
        // 	simulator.run();
        // }
        // catch(const std::exception& e)
        // {
        // 	std::cerr << e.what() << '\n';
        // 	return 1;
        // }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
