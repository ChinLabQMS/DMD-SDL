TEST = gray black white red horizline vertline horizline2 vertline2 blackwhite 
TEST_SRC = test/test.cpp

# Compiler settings
CC = g++
OUTPUT_DIR = bin
INCLUDE_PATHS = -I./include
LIBRARY_PATHS = -L./lib
COMPILER_FLAGS_DEBUG = -g -Wall -mwindows -mconsole
COMPILER_FLAGS_RELEASE = -w -Wl,-subsystem, windows -mwindows
LINKER_FLAGS = -lSDL3 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8

test: COMPILER_FLAGS = $(COMPILER_FLAGS_DEBUG) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
test: $(patsubst %, $(OUTPUT_DIR)/%, $(TEST))

release: COMPILER_FLAGS = $(COMPILER_FLAGS_RELEASE) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
release: