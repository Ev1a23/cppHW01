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

std::mutex m;
std::condition_variable Q_not_full;
std::condition_variable Q_not_empty;
std::deque<std::pair<std::string, AbstractAlgorithm*>> Q;
std::atomic<int> done;



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

MySimulator run_sim(std::string house, AbstractAlgorithm *algo)
{
    MySimulator simulator;
    simulator.readHouseFile(house);
    simulator.setAlgorithm(*algo);
    simulator.run();
}

void start_task()
{
    std::cout << "Thread started, id: " << std::this_thread::get_id() << std::endl;
    while (done == 0)
    {
        std::unique_lock<std::mutex> lk(m);
        Q_not_empty.wait(lk, []{return (!Q.empty() || done);});     // practically waits until 
                                                                    // there's an available task or all tasks are done
        if (!Q.empty())
        {
            std::pair<std::string, AbstractAlgorithm*> task = Q.front();
            Q.pop_front();
            Q_not_full.notify_one(); // awake main's thread to add a task to thread
            lk.unlock();
            run_sim(task.first, task.second);
            std::cout << "Thread " << std::this_thread::get_id() << " ran sim on " << task.first << std::endl;
        }
    }
    std::cout << "Thread finished, id: " << std::this_thread::get_id() << std::endl;
}


int main(int argc, char* argv[]) {

    try
    {
        Config config = parse_args(argc, argv);

        std::cout << "House Path: " << config.house_path << std::endl;
        std::cout << "Algo Path: " << config.algo_path << std::endl;
        std::cout << "Number of Threads: " << config.num_threads << std::endl;
        std::cout << "Summary Only: " << (config.summary_only ? "true" : "false") << std::endl;

        std::vector<fs::path> valid_houses;
        std::vector<std::unique_ptr<AbstractAlgorithm>> algorithms; // this is here just to make the algorithms local of main
                                                                    // so they will be available to simualtors the whole run
        std::vector<std::unique_ptr<std::thread>> threads;
        
        process_algos(config.algo_path);
        process_houses(config.house_path, valid_houses);
        done = 0;
        // create threads with function start_task(house, algo)
        for (int i = 0; i < config.num_threads; i++)
        {
            // start a thread with start_task()
            threads.push_back(std::make_unique<std::thread>(start_task));
            // threads.back()->detach();
        }
        std::cout << "Finished creating threads" << std::endl;

        for (fs::path house : valid_houses)
        {
            for (auto algo : AlgorithmRegistrar::getAlgorithmRegistrar())
            {
                // algo_name = algo.name()
                // house_name = house.file_name()
                algorithms.push_back(algo.create()); // has to save unique ptr here so memory will not be freed
                std::unique_lock lk(m);
                Q_not_full.wait(lk, [&]{return Q.size() < config.num_threads;}); // practically waits until 
                                                                             // there's an available thread
                Q.push_back(std::make_pair(std::string(house.string()), algorithms.back().get())); // add task to Q
                lk.unlock();
                Q_not_empty.notify_one(); // awake a thread waiting for q to be non-empty
            }
        }
        done = 1;
        Q_not_empty.notify_all();
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
