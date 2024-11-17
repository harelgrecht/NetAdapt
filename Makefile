# Define compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lpcap

# Define the target and source directories
TARGET = MUST
SRC_DIR = src
PACKET_CAPTURE_DIR = Packet_Capture
GPIO_HANDLER_DIR = GPIO_Handler
INC_DIR = include

# Define the source and header files
SOURCES = MUST.cpp $(PACKET_CAPTURE_DIR)/Packet_Capture.cpp $(GPIO_HANDLER_DIR)/GPIO_Handler.cpp
HEADERS = $(PACKET_CAPTURE_DIR)/Packet_Capture.hpp $(GPIO_HANDLER_DIR)/GPIO_Handler.hpp

# Define the object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default rule to build the target
compile: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Rule to compile each source file into an object file
# For MUST.cpp
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(PACKET_CAPTURE_DIR) -I$(GPIO_HANDLER_DIR) -c $< -o $@

# For Packet_Capture.cpp
$(PACKET_CAPTURE_DIR)/%.o: $(PACKET_CAPTURE_DIR)/%.cpp $(INC_DIR)/Packet_Capture.hpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# For GPIO_Handler.cpp
$(GPIO_HANDLER_DIR)/%.o: $(GPIO_HANDLER_DIR)/%.cpp $(INC_DIR)/GPIO_Handler.hpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Run the executable
run: $(TARGET)
	./$(TARGET)
