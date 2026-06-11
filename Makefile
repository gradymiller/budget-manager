CXX = g++
CXXFLAGS = -Wall -O2 -std=c++26 -Isrc/core

TARGET = budget
SRC = src/cli/main.cpp src/cli/commands.cpp src/cli/info.cpp src/core/init.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
