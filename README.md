# Final cpp project
## Compilation of each of the projects seperately
In terminal:
	1. cd to project directory
	2. mkdir build
	3. cd build
	4. cmake ..
	5. make
myrobot / .so file will be generated in build
## Compilation of everything together
In terminal of main directory:
	1. chmod +x compiling.bash if needed
	2. ./compiling.bash
This creates the build directory, compiles every of the algorithms/simulator and creates myrobot.
myrobot will be generated in build, and .so files will be generated inside build/algo_dir