# Define the compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Iinclude

# Define the target executable
TARGET = main

# Define the directories for source files and object files
SRCDIR = src
OBJDIR = obj

# Find all source files in the src directory
SRCFILES = $(wildcard $(SRCDIR)/*.cpp)

# Create a list of object files based on the source files
OBJFILES = $(SRCFILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
MAIN_OBJ = $(OBJDIR)/main.o

# The default target
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJFILES) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile each source file into an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Rule to compile the main.cpp file into an object file
$(MAIN_OBJ): main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Rule to create the obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up the build
clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
