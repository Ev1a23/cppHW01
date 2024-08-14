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
#include <chrono>

#include <dlfcn.h>

#include <thread>
#include <condition_variable>
#include <queue>

namespace fs = std::filesystem;

struct Config {
    std::string house_path = fs::current_path().string();
    std::string algo_path = fs::current_path().string();
    int num_threads = 10;
    bool summary_only = false;
};


struct SimArgs {
    fs::path housePath;
    int house_ind;
    AbstractAlgorithm *algo;
    int algo_ind;
    std::string algoName;
};

Config config;
std::mutex Q_lock;
std::condition_variable Q_not_full;
std::condition_variable Q_not_empty;
std::deque<SimArgs> Q;
std::atomic<int> done;
std::mutex summary_lock;
std::vector<std:vector<std::string>> summary;

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

bool validateHouseFile(const fs::path &houseFilePath)
{
    MySimulator sim;
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
        else
        {
            try {(AlgorithmRegistrar::getAlgorithmRegistrar().begin() + algoCountBefore)->create();}
            catch (const std::exception& e) {
                throw std::runtime_error("Algorithm is invalid");
            }
        }
    }
    catch (const std::exception& e)
    {
        handleInvalidFileException(e, algoFilePath);
        return false;
    }
    return true;
}

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

void process_houses(const std::string& house_path, std::vector<fs::path> &valid_houses)
{
    for (const auto& house_entry : fs::directory_iterator(house_path)) 
    {
        if (house_entry.is_regular_file() && house_entry.path().extension() == ".house") 
        {
            if (validateHouseFile(house_entry.path()))
            {
                valid_houses.push_back(house_entry.path());
            }
        }
    }
}

void process_algos(const std::string& algo_path) 
{
    for (const auto& algo_entry : fs::directory_iterator(algo_path)) 
    {
        if (algo_entry.is_regular_file() && algo_entry.path().extension() == ".so") {
            validateAlgoFile(algo_entry.path());
        }
    }
}

MySimulator run_sim(SimArgs &simArgs)
{
    std::string housePath = std::string(simArgs.housePath.string());
    std::string houseName = simArgs.housePath.filename().replace_extension("");
    std::string algoName = simArgs.algoName;
    AbstractAlgorithm *algo = simArgs.algo;
    std::string outputFileName = houseName + "-" + algoName + ".txt";
    MySimulator simulator;
    simulator.readHouseFile(housePath);
    simulator.setAlgorithm(*algo);
    int score = simulator.run();
    if (!config.summary_only)
    {
        std::string output(simulator.getSummaryString());
        std::ofstream outputFile(outputFileName);
        if(!outputFile.is_open())
        {
            throw std::runtime_error("Failed to open output file.");
        }
        outputFile << output << std::endl;
    }
    std::lock_guard<std::mutex> guard(summary_lock);
    scores[][]
}

void init_scores(std::vector<std::unique_ptr<AbstractAlgorithm>> algorithms, std::vector<fs::path> &valid_houses)
{
    std::vector<std::string>> houses;
    houses.push_back("");
    for (auto house: valid_houses)
    {
        houses.push_back(house.filename.replace_extension(""));
    }
    scores.push_back(houses);
}

void add_algo_row(std::string algoName, int col_num)
{
    std::vector<std::string> row;
    row.push_back(algoName);
    for (int i = 0; i < col_num; i++)
    {
        row.push_back("");
    }
}


void start_task()
{
    std::cout << "Thread started, id: " << std::this_thread::get_id() << std::endl;
    while (done == 0)
    {
        std::unique_lock<std::mutex> lk(Q_lock);
        Q_not_empty.wait(lk, []{return (!Q.empty() || done);});     // practically waits until 
                                                                    // there's an available task or all tasks are done
        std::cout << "Thread " << std::this_thread::get_id() << " is awake" << std::endl;
        if (!Q.empty())
        {
            SimArgs simArgs = Q.front();
            Q.pop_front();
            lk.unlock();
            Q_not_full.notify_one(); // awake main's thread to add a task to thread
            run_sim(simArgs);
        }
        else
        {
            lk.unlock();
        }
    }
    std::cout << "Thread finished, id: " << std::this_thread::get_id() << std::endl;
}


int main(int argc, char* argv[]) {

    try
    {
        //////////////////////////////////////////////////////////////////////
        //                       Parsing Arguments                          //
        //////////////////////////////////////////////////////////////////////
        config = parse_args(argc, argv);

        std::cout << "House Path: " << config.house_path << std::endl;
        std::cout << "Algo Path: " << config.algo_path << std::endl;
        std::cout << "Number of Threads: " << config.num_threads << std::endl;
        std::cout << "Summary Only: " << (config.summary_only ? "true" : "false") << std::endl;


        //////////////////////////////////////////////////////////////////////
        //                        Validating Arguments                      //
        //////////////////////////////////////////////////////////////////////
        std::vector<fs::path> valid_houses;
        std::vector<std::unique_ptr<AbstractAlgorithm>> algorithms;        
        process_algos(config.algo_path);
        process_houses(config.house_path, valid_houses);
        init_scores();

        done = 0; // set before starting threads
        //////////////////////////////////////////////////////////////////////
        //                          Start Threads                           //
        //////////////////////////////////////////////////////////////////////
        std::vector<std::unique_ptr<std::thread>> threads; // keep threads object on heap
        for (int i = 0; i < config.num_threads; i++)
        {
            threads.push_back(std::make_unique<std::thread>(start_task));
        }
        std::cout << "Finished creating threads" << std::endl;


        //////////////////////////////////////////////////////////////////////
        //          Fill Q with tasks - one per <house, algo> pair          //
        //////////////////////////////////////////////////////////////////////
        algo_ind = 0;
        for (auto algo : AlgorithmRegistrar::getAlgorithmRegistrar())
        {
            house_ind = 0;
            add_algo_row(algo.name());
            for (fs::path house : valid_houses)
            {
                algorithms.push_back(algo.create()); // save unique ptr in main's stack so algo object won't be freed
                std::unique_lock lk(Q_lock);
                Q_not_full.wait(lk, [&]{return Q.size() < config.num_threads;});    // practically waits until there's an available thread
                Q.push_back(SimArgs{house, house_ind, algorithms.back().get(), algo_ind, algo.name()}); //std::make_pair(house, algorithms.back().get())); // add task to Q
                lk.unlock();
                Q_not_empty.notify_one(); // awake a waiting thread
                ++house_ind;
            }
            ++algo_ind;
        }
        done = 1;   // set before notifying all - 
                    // threads won't start another loop, but if there's a task in Q they will handle it
        Q_not_empty.notify_all(); // awake sleeping all threads


        //////////////////////////////////////////////////////////////////////
        //                          Join Threads                            //
        //////////////////////////////////////////////////////////////////////
        for (int i =0; i < threads.size(); i++)
        {
            threads[i]->join();
        }
    }
    
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
