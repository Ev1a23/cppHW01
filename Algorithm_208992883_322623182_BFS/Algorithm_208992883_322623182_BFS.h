#ifndef BFS_ALGORITHM_H
#define BFS_ALGORITHM_H

#include "../custom_algorithm/MyAlgorithm.h"
#include "../algorithm/AlgorithmRegistration.h"

class Algorithm_208992883_322623182_BFS : public MyAlgorithm {
public:
    Algorithm_208992883_322623182_BFS();
    virtual ~Algorithm_208992883_322623182_BFS();
    virtual Step nextStep();

private:
	std::queue<std::pair<int, int>> bfsQueue;
    void initializeGrid();
    void updateLoc();
    bool isAtDockingStation() const;
    Step handleChargingLogic();
    bool canMoveTo(const std::pair<int, int>& pos) const;
    Step getStepTowards(const std::pair<int, int>& pos);
    Step returnToDocking();
    void enqueueNeighbors();
    Step moveTowardsTarget();
	std::deque<Step> calculatePath(std::pair<int,int> source, std::pair<int,int> target);
	std::deque<Step> calculatePathToDocking(std::pair<int,int> source);	

};

#endif // BFS_ALGORITHM_H
