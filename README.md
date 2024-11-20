# Final cpp project
## Compilation of each of the projects seperately
In terminal:
	1. cd to project directory
	2. mkdir build
	3. cd build
	4. cmake ..
	5. make
myrobot / .so file will be generated in build
## Compilation of everything together using script
In terminal of main directory:
	1. chmod +x compiling.bash if needed
	2. ./compiling.bash
This creates the build directory, compiles every of the algorithms/simulator and creates myrobot.
myrobot will be generated in build, and .so files will be generated inside build/algo_dir

## Compilation of everything together using cmake and make
In terminal of main directory:
	1. mkdir build
	2. cd build
	3. cmake ..
	4. make

The custom_algorithm dir is for shared code of our 2 algorithms.
So 2 .so files will be generated after compiling everything:
Algorithm_208992883_322623182_BFS.so
Algorithm_208992883_322623182_randomized_DFS.so