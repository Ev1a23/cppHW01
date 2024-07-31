#include "AlgorithmRegistrar.h"

static AlgorithmRegistrar& AlgorithmRegistrar::getAlgorithmRegistrar()
{
    return registrar;
}