# cppHW01

## Conventions
* Each row in the file represents positions.
* W denotes a wall.
* D denotes the docking station.
* Numbers 0-9 represent dirt levels.
* Unknown chars will be used as spaces.
* Walls will always be added to make sure the house is wrapped.

## Overview
* If the input does not adhere to these conventions, a specific error will be thrown.
* Otherwise, the MySimulator manages the entire process by asking the algorithm for its next step and updating the house accordingly.
* The algorithm starts exploring the house by prioritizing (Known Dirt Level > 0) > (Unknown non wall) > (Known Clean Position),
and returning to docking station with the known shortest path when needed based on battery level / max total steps.