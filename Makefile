SRC_DIR = src
OUTPUT_DIR = bin

# Compiler settings
CC = g++
INCLUDE_PATHS = -I./include
LIBRARY_PATHS = -L./lib
CFLAGS_TEST = -Wall
CFLAGS_STANDARD = -Wall -DTEST=0
LINKER_FLAGS = -lSDL3 -lm -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8

COMPILER_FLAGS_TEST = $(CFLAGS_TEST) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
COMPILER_FLAGS_STANDARD = $(CFLAGS_STANDARD) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

all: static black_white static_example

$(OUTPUT_DIR)/%_test.exe: $(SRC_DIR)/%.cpp $(SRC_DIR)/PatternWindow.cpp
	@echo "Building target: $@"
	$(CC) $^ -o $@ $(COMPILER_FLAGS_TEST)

$(OUTPUT_DIR)/%.exe: $(SRC_DIR)/%.cpp $(SRC_DIR)/PatternWindow.cpp
	@echo "Building target: $@"
	$(CC) $^ -o $@ $(COMPILER_FLAGS_STANDARD)

static: $(OUTPUT_DIR)/static_test.exe $(OUTPUT_DIR)/static.exe
static_example: $(OUTPUT_DIR)/static_example_test.exe $(OUTPUT_DIR)/static_example.exe
black_white: $(OUTPUT_DIR)/black_white_test.exe $(OUTPUT_DIR)/black_white.exe

.PHONY: all static black_white