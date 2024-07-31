CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -Wextra -Werror -pedantic
TARGET = myrobot
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean

all: clean $(TARGET)

# Compile the target from all object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp MySimulator.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

MySimulator.o: MySimulator.cpp House.o MyAlgorithm.o RandomizedStepAlgorithm.o BFSAlgorithm.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

House.o: House.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

MyAlgorithm.o: MyAlgorithm.cpp abstract_algorithm.h my_wall_sensor.o my_dirt_sensor.o my_battery_meter.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

my_wall_sensor.o: my_wall_sensor.cpp wall_sensor.h enums.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

my_dirt_sensor.o: my_dirt_sensor.cpp dirt_sensor.h enums.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

my_battery_meter.o: my_battery_meter.cpp battery_meter.h enums.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

RandomizedStepAlgorithm.o: RandomizedStepAlgorithm.cpp MyAlgorithm.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

BFSAlgorithm.o: BFSAlgorithm.cpp MyAlgorithm.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
