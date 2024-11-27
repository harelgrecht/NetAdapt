# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 
INCLUDES = -I./Includes

# Libraries to link against
LIBS = -lpcap -lz

# Source and output directories
SRC_DIR = Sources
OBJ_DIR = obj
BIN_DIR = bin

# Executable name (change to reflect "MUST")
TARGET = $(BIN_DIR)/MUST

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: compile

# Compile target
compile: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run target
run: $(TARGET)
	@./$(TARGET)

# Clean target
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned up build files."
