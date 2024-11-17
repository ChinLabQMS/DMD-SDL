SRC_DIR = src
OUTPUT_DIR = bin

# Compiler settings
CC = g++
INCLUDE_PATHS = -I./include
LIBRARY_PATHS = -L./lib
CFLAGS_DEBUG = -g -Wall -mwindows -mconsole -DDEBUG
CFLAGS_RELEASE = -w -mwindows -mconsole -DDEBUG=0
LINKER_FLAGS = -lSDL3 -lm -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8

COMPILER_FLAGS_DEBUG = $(CFLAGS_DEBUG) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
COMPILER_FLAGS_RELEASE = $(CFLAGS_RELEASE) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

all: static

static: $(SRC_DIR)/static.cpp
	$(CC) $(SRC_DIR)/static.cpp -o $(OUTPUT_DIR)/static_debug.exe $(COMPILER_FLAGS_DEBUG)
	$(CC) $(SRC_DIR)/static.cpp -o $(OUTPUT_DIR)/static.exe $(COMPILER_FLAGS_RELEASE)
