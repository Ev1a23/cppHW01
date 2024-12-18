#ifndef RANDOMIZED_STEP_ALGORITHM_H
#define RANDOMIZED_STEP_ALGORITHM_H

#include "../custom_algorithm/MyAlgorithm.h"
#include "../algorithm/AlgorithmRegistration.h"

class Algorithm_208992883_322623182_randomized_DFS : public MyAlgorithm {
public:
    Algorithm_208992883_322623182_randomized_DFS();
    Step nextStep() override;
	Direction RandomizedNeighborsHandle();
	Direction ChooseNextStep();
	bool comparePositions(Position positionA, Position positionB) const;
};

#endif // RANDOMIZED_STEP_ALGORITHM_H
