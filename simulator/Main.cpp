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
#include <pthread.h>
#include <condition_variable>
#include <queue>

#define THREAD_PRINT (1)

#if THREAD_PRINT
#define THREADING_LOG(x, y, z) thread_print(x, y, z)
#else
#define THREADING_LOG(x, y, z) {}
#endif


namespace fs = std::filesystem;

struct Config {
    std::string house_path = fs::current_path().string();
    std::string algo_path = fs::current_path().string();
    int num_threads = 10;
    bool summary_only = false;
};


struct SimArgs {
    House &house;
    int house_ind;
    AbstractAlgorithm *algo;
    int algo_ind;
    std::string algoName;

    std::string getOutputFile() {return house.fileName() + "-" + algoName + ".txt";};
    MySimulator createSim() {
        MySimulator simulator;
        simulator.setHouse(house);
        simulator.setAlgorithm(*algo);
        return simulator;
    }
};

Config config;
std::mutex Q_lock;
// std::condition_variable Q_not_full;
std::condition_variable Q_not_empty;
std::deque<SimArgs> Q;
std::atomic<int> done;
std::mutex summary_lock;
std::vector<std::vector<std::string>> summary;

void handleInvalidFileException(const std::exception &e, const fs::path &invalidFilePath)
{
    std::cout << invalidFilePath.string() << std::endl;
    fs::path errorFilename = invalidFilePath.filename().replace_extension("error");
    errorFilename = errorFilename.string().compare(0,3,"lib") == 0 ? fs::path(errorFilename.string().substr(3)) : errorFilename;
    fs::path errorPath = fs::current_path().string() / errorFilename;
    std::cout << errorPath.string() << std::endl;
    std::ofstream errorFile(errorPath);
    if (!errorFile) {
        std::cerr << "Failed to open file: " << invalidFilePath << std::endl;
        throw std::runtime_error("Could not write error file");
    }
    errorFile <<  e.what() << std::endl;
}

void validateHouseFile(const fs::path &houseFilePath, std::vector<House> &valid_houses)
{
    try
    {
        House h(houseFilePath);
        valid_houses.push_back(h);
    }
    catch (const std::exception& e)
    {
        handleInvalidFileException(e, houseFilePath);
    }
}

bool validateAlgoFile(const fs::path &algoFilePath, std::vector<void*> &algoPointers)
{
	std::cout << "Validating algo file: " << algoFilePath << "\n";
    size_t algoCountBefore = AlgorithmRegistrar::getAlgorithmRegistrar().count(); // how many algorithm are registered so far
    void *algoFilePtr;
    try
    {
        algoFilePtr = dlopen(algoFilePath.string().c_str(), RTLD_LAZY);
		if(algoFilePtr)
		{
			algoPointers.push_back(algoFilePtr);
		}
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
            if (config.num_threads < 1)
                {throw std::runtime_error("Illegal num_threads parameter: " + std::to_string(config.num_threads));}
			config.num_threads = std::min(config.num_threads, 2 * static_cast<int>(std::thread::hardware_concurrency()));
			std::cout << "Using " << config.num_threads << " threads" << std::endl;

        } else if (arg == "-summary_only") {
            config.summary_only = true;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
        }
    }

    return config;
}

void process_houses(const std::string& house_path, std::vector<House> &valid_houses)
{
    for (const auto& house_entry : fs::directory_iterator(house_path)) 
    {
        if (house_entry.is_regular_file() && house_entry.path().extension() == ".house") 
        {
            validateHouseFile(house_entry.path(), valid_houses);
        }
    }
}

void process_algos(const std::string& algo_path, std::vector<void*> &algoPointers) 
{
    for (const auto& algo_entry : fs::directory_iterator(algo_path)) 
    {
        if (algo_entry.is_regular_file() && algo_entry.path().extension() == ".so") {
            validateAlgoFile(algo_entry.path(), algoPointers);
        }
    }
}


void init_scores(std::vector<House> &valid_houses)
{
    std::vector<std::string> houses;
    houses.push_back("");
    for (auto house: valid_houses)
    {
        houses.push_back(house.fileName());
    }
    summary.push_back(houses);
}

void add_algo_row(std::string algoName, int col_num)
{
    std::vector<std::string> row;
    row.push_back(algoName);
    for (int i = 0; i < col_num; i++)
    {
        row.push_back("");
    }
    summary.push_back(row);
}

void write_summary()
{
    std::ofstream summaryFile("summary.csv");
	if(!summaryFile.is_open())
	{
		throw std::runtime_error("Failed to open output file.");
	}
    for (auto row : summary)
    {
        int i;
        for (i = 0; i < row.size() - 1; i++)
        {
            summaryFile << row[i] << ",";
        }
        summaryFile << row[i] << "\n";
    }
    summaryFile.close();
}

void runSim(MySimulator *sim, std::atomic<bool> *finished, std::condition_variable *cv_timeout, std::mutex *m)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);
    std::unique_lock l(*m);     // make sure the calling thread has started waiting
    std::cout << "Thread " << std::this_thread::get_id() << " is running a simulation" << std::endl;
    l.unlock();                 // make sure the calling thread won't block if the simulation blocks
    sim->run(*m);
    l.lock();
    *finished = true;
    l.unlock();
    cv_timeout->notify_one();   // wake up calling thread in case of timeout has not reached yet
}


void beginRunSim(SimArgs &simArgs)
{
    MySimulator::SimResults results;
    std::string outputFilePath = simArgs.getOutputFile();
    MySimulator sim = simArgs.createSim();
    int score;
    std::size_t initialDirt = simArgs.house.totalDirt();
    std::size_t maxSteps = simArgs.house.getMaxSteps();

    std::atomic<bool> finished = false;
    std::condition_variable timeout_cv;
    std::mutex m;
    std::unique_lock l(m); // make sure t doesnt start the simulation before we start waiting
    std::thread t(runSim, &sim, &finished, &timeout_cv, &m);
    std::cout << "Started a thread for simulation sun, thread_id = " << t.get_id() << std::endl;
    if (!timeout_cv.wait_for(l, std::chrono::milliseconds(2*maxSteps), [&finished]() { return finished.load(); })) {
        // Timeout occurred
        pthread_cancel(t.native_handle());
        t.detach();
        std::cout << "Timeout for " << outputFilePath << std::endl;
        results = sim.getResults();
        results.score = maxSteps * 2 + initialDirt * 300 + 2000;
        results.stepsLog += "T";
        l.unlock();
    }
    else
    {
        t.join();
        std::cout << "Finished sim " << outputFilePath << " within timeout\n";
        results = sim.getResults();
    }
    if (!config.summary_only)
    {
        std::string output(results.str());
        std::ofstream outputFile(outputFilePath);
        if(!outputFile.is_open())
        {
            throw std::runtime_error("Failed to open output file.");
        }
        outputFile << output << std::endl;
    }
    std::lock_guard<std::mutex> guard(summary_lock);
    summary[simArgs.algo_ind + 1][simArgs.house_ind + 1] = std::to_string(results.score);   // +1 because first row and first 
                                                                                    // col are for algos and houses names
}

void thread_print(auto thread_id, fs::path house, std::string algo) {
    std::cout << "Thread " << thread_id << " handles (add / remove) a new task" << std::endl;
    std::cout << "\t house: " << house << std::endl;
    std::cout << "\t algo: " << algo << std::endl;
    std::cout << "\t #task_remained = " << Q.size() << std::endl;
}

void start_task()
{
    std::cout << "Thread started, id: " << std::this_thread::get_id() << std::endl;
    while (!Q.empty() || done == 0)
    {
        std::unique_lock<std::mutex> lk(Q_lock); // lock here because while condition access Q
        Q_not_empty.wait(lk, []{return (!Q.empty() || done);});     // practically waits until 
                                                                    // there's an available task or all tasks are done
        if (!Q.empty())
        {
            SimArgs simArgs = Q.front();
            Q.pop_front();
            lk.unlock();
            beginRunSim(simArgs);
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
        std::vector<House> valid_houses;
        std::vector<std::unique_ptr<AbstractAlgorithm>> algorithms;
		std::vector<void*> algoPointers = {};
        process_algos(config.algo_path, algoPointers);
        process_houses(config.house_path, valid_houses);
        init_scores(valid_houses);

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
        int algo_ind = 0;
        for (auto algo : AlgorithmRegistrar::getAlgorithmRegistrar())
        {
            int house_ind = 0;
            add_algo_row(algo.name(), valid_houses.size());
            for (int i = 0; i < valid_houses.size(); i++)
            {
                algorithms.push_back(algo.create()); // save unique ptr in main's stack so algo object won't be freed
                std::unique_lock lk(Q_lock);
                Q.push_back(SimArgs{valid_houses[i], house_ind, algorithms.back().get(), algo_ind, algo.name()}); //std::make_pair(house, algorithms.back().get())); // add task to Q
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


        write_summary();
		//////////////////////////////////////////////////////////////////////
        //                          Dlclose on opened algos                 //
        //////////////////////////////////////////////////////////////////////
		algorithms.clear();
		AlgorithmRegistrar::getAlgorithmRegistrar().clear();
		for (auto p : algoPointers)
		{
			dlclose(p);
		}
		std::cout<<"Finished closing all algo handles"<<"\n";
    }
    
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
