# cppHW01

## Conventions
* The input file should be a comma-separated text file.
* Each row in the file represents positions.
* -1 denotes a wall.
* 10 denotes the docking station.
* Numbers 0-9 represent dirt levels.
* Walls will always be added to make sure the house is wrapped.

## Overview
* If the input does not adhere to these conventions, a specific error will be thrown.
* Otherwise, the Simulation Controller manages the entire process by asking the algorithm for its next step and updating the house accordingly.
* The algorithm starts by saving a path from the docking station and checks if it needs to return due to battery constraints or completion. Otherwise, it deterministically chooses the next move based on the current dirt level and the presence of walls, prioritizing directions in the order: North, East, South, West.
* This deterministic decision-making allows us to consistently test our code, ensuring the output remains the same for each run with identical input.