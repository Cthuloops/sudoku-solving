# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
BUILD_DIR = build
SRC_DIR = src
TARGET = sudoku

# Source and object files
SOURCES = main.c grid.c cell.c
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Declare phony targets
.PHONY: all clean
