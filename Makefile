# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./Includes
LIBFLAGS = -lz -lpcap

# Directories
SRCDIR = Sources
INCDIR = Includes
OBJDIR = obj
BINDIR = bin

# Source and Object Files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Targets
NORMAL_OUTPUT = $(BINDIR)/MUST.o
MOCK_OUTPUT = $(BINDIR)/MOCK_MUST.o

# Ensure directories exist
$(OBJDIR) $(BINDIR):
	mkdir -p $@

# Clean up build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Normal compilation (without MOCK_UP)
normal_compile: $(OBJDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(NORMAL_OUTPUT) $(LIBFLAGS)

# Mock compilation (with MOCK_UP)
mock_compile: $(OBJDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) -DMOCK_UP $(SRCS) -o $(MOCK_OUTPUT) $(LIBFLAGS)

# Run the compiled program
normal_run: $(NORMAL_OUTPUT)
	sudo ./$(NORMAL_OUTPUT)

mock_run: $(MOCK_OUTPUT)
	sudo ./$(MOCK_OUTPUT)