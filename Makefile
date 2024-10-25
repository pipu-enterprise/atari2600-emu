# Compiler and flags
CC = gcc
CFLAGS = -Iinc -Wall -Wextra -Werror -std=c11

# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# Target executable name
TARGET = main

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Rule to compile .c files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up object files and executable
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

# Run the program
run: all
	./$(TARGET)

# Phony targets
.PHONY: all clean run
