# Define compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lpcap

# Define the target and source directories
TARGET = MUST
SRC_DIR = src
GPIO_HANDLER_DIR = GPIO_Handler
KEEP_RUNNING_DIR = Keep_Running
PACKET_CAPTURE_DIR = Packet_Capture
PACKET_PROCESS_DIR = Packet_Process
PACKET_SEND_DIR = Packet_Send
QUEUE_DIR = Queue

# Define the source and header files
SOURCES = $(SRC_DIR)/MUST.cpp \
          $(GPIO_HANDLER_DIR)/GPIO_Handler.cpp \
          $(KEEP_RUNNING_DIR)/Keep_Running.cpp \
          $(PACKET_CAPTURE_DIR)/Packet_Capture.cpp \
          $(PACKET_PROCESS_DIR)/Packet_Process.cpp \
          $(PACKET_SEND_DIR)/Packet_Send.cpp \
          $(QUEUE_DIR)/Queue.cpp

HEADERS = $(SRC_DIR)/MUST.hpp \
          $(GPIO_HANDLER_DIR)/GPIO_Handler.hpp \
          $(KEEP_RUNNING_DIR)/Keep_Running.hpp \
          $(PACKET_CAPTURE_DIR)/Packet_Capture.hpp \
          $(PACKET_PROCESS_DIR)/Packet_Process.hpp \
          $(PACKET_SEND_DIR)/Packet_Send.hpp \
          $(QUEUE_DIR)/Queue.hpp

# Define the object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Rule to compile each source file into an object file
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(GPIO_HANDLER_DIR) -I$(KEEP_RUNNING_DIR) \
		-I$(PACKET_CAPTURE_DIR) -I$(PACKET_PROCESS_DIR) -I$(PACKET_SEND_DIR) \
		-I$(QUEUE_DIR) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Run the executable
run: $(TARGET)
	./$(TARGET)
