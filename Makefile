CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Werror -pedantic
TARGET=myrobot
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

# Compile the target from all object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rules for individual object files
VacuumCleaner.o: VacuumCleaner.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp SimulationController.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

Algorithm.o: Algorithm.cpp SensorSystem.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

FileManagement.o: FileManagement.cpp House.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

SensorSystem.o: SensorSystem.cpp House.o VacuumCleaner.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

SimulationController.o: SimulationController.cpp VacuumCleaner.o SensorSystem.o House.o Algorithm.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

House.o: House.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
