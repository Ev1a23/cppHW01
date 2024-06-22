CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Werror -pedantic
TARGET=myrobot
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

all:$(TARGET)
	
$(TARGET):$(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
	
