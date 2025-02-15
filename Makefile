# Project Settings
SRC_DIR := ./src
BUILD_DIR := ./build
INCLUDE_DIR := ./include
LIB_DIR := ./lib
BIN_DIR := ./bin

# Object Files
OBJS := re8bit.o chip8.o window.o beep.o

# Compiler Settings
CC := clang
CFLAGS := -std=c2x -Wall -Wextra -pedantic -I$(INCLUDE_DIR) -O3
CFLAGS += -lcsfml-graphics -lcsfml-system -lcsfml-audio -Wno-unused-command-line-argument

# Build re8bit
re8bit: dir $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/re8bit $(foreach file,$(OBJS),$(BUILD_DIR)/$(file))

# Build Object Files
$(OBJS): dir
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c

# Setup build and bin directories
dir:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Clean build and bin directories
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

bear:
	bear -- make

.PHONY: all clean
