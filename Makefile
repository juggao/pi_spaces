# Makefile for Pi Processor

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -O3

# Linker flags
LDFLAGS = -lgmp -lmpfr

# Target executable
TARGET = pi_processor

# Source files
SOURCES = pi_processor.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) pi_digits.txt pi_replaced.txt pi_colored.txt

# Clean only output files
clean-output:
	rm -f pi_digits.txt pi_replaced.txt pi_colored.txt

# Install dependencies (for Debian/Ubuntu)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libgmp-dev libmpfr-dev

# Help
help:
	@echo "Available targets:"
	@echo "  all           - Build the program (default)"
	@echo "  run           - Build and run the program"
	@echo "  clean         - Remove build artifacts and output files"
	@echo "  clean-output  - Remove only output files"
	@echo "  install-deps  - Install required libraries (Ubuntu/Debian)"
	@echo "  help          - Show this help message"

.PHONY: all run clean clean-output install-deps help
