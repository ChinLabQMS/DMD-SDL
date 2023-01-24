TEST = win_gray win_black win_white win_red win_hori_line win_vert_line\
		win_hori_line2 win_vert_line2 win_black_white

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

$(OUTPUT_DIR)/win_gray: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=1 -o $@

$(OUTPUT_DIR)/win_black: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=2 -o $@

$(OUTPUT_DIR)/win_white: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=3 -o $@

$(OUTPUT_DIR)/win_red: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=4 -o $@

$(OUTPUT_DIR)/win_hori_line: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=5 -o $@

$(OUTPUT_DIR)/win_vert_line: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=6 -o $@

$(OUTPUT_DIR)/win_hori_line2: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=7 -o $@

$(OUTPUT_DIR)/win_vert_line2: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=8 -o $@

$(OUTPUT_DIR)/win_black_white: test/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=9 -o $@