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

$(OUTPUT_DIR)/gray: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=1 -o $@

$(OUTPUT_DIR)/black: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=2 -o $@

$(OUTPUT_DIR)/white: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=3 -o $@

$(OUTPUT_DIR)/red: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=4 -o $@

$(OUTPUT_DIR)/horizline: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=5 -o $@

$(OUTPUT_DIR)/vertline: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=6 -o $@

$(OUTPUT_DIR)/horizline2: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=7 -o $@

$(OUTPUT_DIR)/vertline2: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=8 -o $@

$(OUTPUT_DIR)/blackwhite: $(TEST_SRC)
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=9 -o $@