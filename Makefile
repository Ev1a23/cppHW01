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

main.o: main.cpp SimulationController.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

Algorithm.o: Algorithm.cpp House.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

SimulationController.o: SimulationController.cpp House.o Algorithm.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

House.o: House.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
