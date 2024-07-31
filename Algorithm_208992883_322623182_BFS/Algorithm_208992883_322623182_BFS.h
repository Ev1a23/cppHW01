#ifndef BFS_ALGORITHM_H
#define BFS_ALGORITHM_H

#include "../algorithm/MyAlgorithm.h"
#include <queue>
#include <utility>
#include <unordered_set>

class Algorithm_208992883_322623182_BFS : public MyAlgorithm {
public:
    Algorithm_208992883_322623182_BFS();
    virtual ~Algorithm_208992883_322623182_BFS();
	void updateLoc();

    Step nextStep() override;

private:
    std::queue<Position> explorationQueue;

    void enqueueNeighbors(const Position& current);
    Step translateToStep(const Position& from, const Position& to);
};

#endif // BFS_ALGORITHM_H
