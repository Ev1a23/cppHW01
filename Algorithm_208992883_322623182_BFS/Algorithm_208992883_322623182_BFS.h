#ifndef BFS_ALGORITHM_H
#define BFS_ALGORITHM_H

#include "../custom_algorithm/MyAlgorithm.h"
#include "../algorithm/AlgorithmRegistration.h"
#include <queue>
#include <unordered_set>

class Algorithm_208992883_322623182_BFS : public MyAlgorithm {
public:
    Algorithm_208992883_322623182_BFS();
    virtual ~Algorithm_208992883_322623182_BFS();
    virtual Step nextStep();

private:
    std::queue<std::pair<int, int>> bfsQueue;
    std::unordered_set<size_t> visited;
	std::deque<Step> pathToTarget;

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
	std::deque<Step> calculatePathFromAncestor(std::pair<int,int> target, std::pair<int,int> ancestor);

};

#endif // BFS_ALGORITHM_H
