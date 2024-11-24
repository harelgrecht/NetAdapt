# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lpcap

# Directories
SRC_DIRS = src GPIO_Handler Keep_Running Packet_Capture Packet_Process Packet_Send Queue
INCLUDE_DIRS = $(addprefix -I, $(SRC_DIRS))

# Find source and header files dynamically
SOURCES = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
HEADERS = $(wildcard $(addsuffix /*.hpp, $(SRC_DIRS)))
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = MUST

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Run the executable
run: $(TARGET)
	./$(TARGET)
