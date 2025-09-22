# Makefile for C TCP Port Scanner
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
DEBUG_FLAGS = -g -DDEBUG
TARGET = port_scanner
SOURCE = main.c

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCE)
	@echo "Building $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo "Build complete!"

# Debug build
debug: $(SOURCE)
	@echo "Building debug version..."
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET)_debug $(SOURCE)
	@echo "Debug build complete!"

# Install to system (optional)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)
	@echo "Installation complete!"

# Uninstall from system
uninstall:
	@echo "Removing $(TARGET) from /usr/local/bin..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstallation complete!"

# Clean build files
clean:
	@echo "Cleaning up build files..."
	rm -f $(TARGET) $(TARGET)_debug *.o
	@echo "Clean complete!"

# Run tests
test: $(TARGET)
	@echo "Running basic tests..."
	@echo "Test 1: Help message"
	./$(TARGET) -h
	@echo "\nTest 2: Scanning localhost common ports"
	./$(TARGET) localhost 80 80 || true
	@echo "\nTest 3: Invalid arguments"
	./$(TARGET) || true
	@echo "Tests complete!"

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the port scanner (default)"
	@echo "  debug    - Build debug version"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  clean    - Remove build files"
	@echo "  test     - Run basic tests"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all debug install uninstall clean test help