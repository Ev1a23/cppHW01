#ifndef BFS_ALGORITHM_H
#define BFS_ALGORITHM_H

#include "MyAlgorithm.h"
#include <queue>
#include <utility>
#include <unordered_set>

class BFSAlgorithm : public MyAlgorithm {
public:
    BFSAlgorithm();
    virtual ~BFSAlgorithm();
	void updateLoc();

    Step nextStep() override;

private:
    std::queue<Position> explorationQueue;

    void enqueueNeighbors(const Position& current);
    Step translateToStep(const Position& from, const Position& to);
};

#endif // BFS_ALGORITHM_H
