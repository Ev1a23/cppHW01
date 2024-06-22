CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Werror -pedantic
TARGET=myrobot
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
 
VacuumCleaner.o:
	$(CXX) $(CXXFLAGS) -o $@
 
all:$(TARGET)
    
$(TARGET): main.o
	$(CXX) $(CXXFLAGS) -o $@ $^
 
main.o: SimulationController.o
	$(CXX) $(CXXFLAGS) -o Main.cpp
 
 
Algoritm.o: SensorSystem.o
	$(CXX) $(CXXFLAGS) -o Algoritm.cpp $^
FileManagement.o: House.o
	$(CXX) $(CXXFLAGS) -o FileManagement.cpp $^
Main.o: SimulationController.o
	$(CXX) $(CXXFLAGS) -o Main.cpp $^
SensorSystem.o: House.o VacuumCleaner.o
	$(CXX) $(CXXFLAGS) -o SensorSystem.cpp $^
SimulationController.o: VacuumCleaner.o SensorSystem.o House.o Algoritm.o
	$(CXX) $(CXXFLAGS) -o SimulationController.cpp $^
