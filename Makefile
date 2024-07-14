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

main.o: main.cpp MySimulator.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

Algorithm.o: Algorithm.cpp wall_sensor.h dirt_sensor.h battery_meter.h abstract_algorithm.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

MySimulator.o: MySimulator.cpp House.o Algorithm.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

House.o: House.cpp my_wall_sensor.o my_dirt_sensor.o my_battery_meter.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

my_wall_sensor.o: my_wall_sensor.cpp wall_sensor.h enums.h

my_dirt_sensor.o: my_dirt_sensor.cpp dirt_sensor.h enums.h

my_battery_meter.o: my_battery_meter.cpp battery_meter.h enums.h

# wall_sensor.o: wall_sensor.h enums.h

# dirt_sensor.o: dirt_sensor.h enums.h

# battery_meter.o: battery_meter.h enums.h

# abstract_algorithm.o: abstract_algorithm.h


clean:
	rm -f $(OBJ) $(TARGET)
