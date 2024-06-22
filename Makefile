CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Werror -pedantic
TARGET=myrobot
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

# Corrected rules for compiling object files
VacuumCleaner.o: VacuumCleaner.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Corrected dependency and compilation command
main.o: main.cpp SimulationController.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

Algoritm.o: Algoritm.cpp SensorSystem.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

FileManagement.o: FileManagement.cpp House.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

# This rule seems to be a duplicate of the main.o rule and should be removed or corrected
# Main.o: SimulationController.o
# 	$(CXX) $(CXXFLAGS) -o Main.cpp $^

SensorSystem.o: SensorSystem.cpp House.o VacuumCleaner.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

SimulationController.o: SimulationController.cpp VacuumCleaner.o SensorSystem.o House.o Algoritm.o
	$(CXX) $(CXXFLAGS) -c $< -o $@