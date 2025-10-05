# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = test
TARGET = sudoku

# Source files for main program
SOURCES = main.c grid.c cell.c
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

# Implementation files (everything except main.c)
IMPL_SOURCES = grid.c cell.c
IMPL_OBJECTS = $(IMPL_SOURCES:%.c=$(BUILD_DIR)/%.o)

# Test files - finds all .c files in test directory
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
# Transforms test/test_cell.c into build/test_cell
TEST_EXECUTABLES = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SOURCES))

# Default target - builds main program
all: $(TARGET)

# Link object files into main executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile main program .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Test target - builds all test executables
test: $(TEST_EXECUTABLES)

# Pattern rule: build/test_name depends on build/test_name.o and implementation objects
$(BUILD_DIR)/test_%: $(BUILD_DIR)/test_%.o $(IMPL_OBJECTS)
	$(CC) $(CFLAGS) $^ -lcriterion -o $@

# Compile test .c files to .o files
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/test_%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Declare phony targets
.PHONY: all test clean
