#ifndef RANDOMIZED_STEP_ALGORITHM_H
#define RANDOMIZED_STEP_ALGORITHM_H

#include "MyAlgorithm.h"

class RandomizedStepAlgorithm : public MyAlgorithm {
public:
    RandomizedStepAlgorithm();
    Step nextStep() override;
	Direction RandomizedNeighborsHandle();
};

#endif // RANDOMIZED_STEP_ALGORITHM_H