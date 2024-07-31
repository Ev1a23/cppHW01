#ifndef RANDOMIZED_STEP_ALGORITHM_H
#define RANDOMIZED_STEP_ALGORITHM_H

#include "../algorithm/MyAlgorithm.h"

class Algorithm_208992883_322623182_randomized_DFS : public MyAlgorithm {
public:
    Algorithm_208992883_322623182_randomized_DFS();
    Step nextStep() override;
	Direction RandomizedNeighborsHandle();
};

#endif // RANDOMIZED_STEP_ALGORITHM_H
