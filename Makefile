CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g

# Find all cpp files in current directory
SRCS = $(wildcard *.cpp)

# Generate object file names
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = main

# Default rule
all: $(TARGET)

# Rule to link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile cpp files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	del /Q /F *.o $(TARGET).exe 2>NUL || rm -f *.o $(TARGET) $(TARGET).exe

.PHONY: all clean
