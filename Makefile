# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./Includes

# Directories
SRCDIR = Sources
INCDIR = Includes
OBJDIR = obj
BINDIR = bin

# Source Files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
# Object Files
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
# Target Executable
TARGET = $(BINDIR)/MUST

# Ensure directories exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Default Target
all: $(TARGET)

# Linking Target Executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lpcap -lz

# Compile Each Source File into Object File
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Up
clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)

run:
	sudo ./bin/MUST

# Phony Targets
.PHONY: all clean
