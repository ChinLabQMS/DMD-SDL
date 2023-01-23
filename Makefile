OUTPUT = win_gray win_hori_line win_vert_line win_cross_line win_hori_line2 win_vert_line2 win_cross_line2

OUTPUT_DIR = bin
OUTPUT_NAME = $(patsubst %, $(OUTPUT_DIR)/%, $(OUTPUT))

CC = g++
INCLUDE_PATHS = -I./include
LIBRARY_PATHS = -L./lib
COMPILER_FLAGS_DEBUG = -g -Wall -mwindows -mconsole
COMPILER_FLAGS_RELEASE = -w -Wl,-subsystem, windows -mwindows
LINKER_FLAGS = -lSDL3 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8

debug: COMPILER_FLAGS = $(COMPILER_FLAGS_DEBUG) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
debug: $(OUTPUT_NAME)

release: COMPILER_FLAGS = $(COMPILER_FLAGS_RELEASE) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
release: $(OUTPUT_NAME)

$(OUTPUT_DIR)/win_gray: src/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=1 -o $@

$(OUTPUT_DIR)/win_hori_line: src/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=2 -o $@

$(OUTPUT_DIR)/win_vert_line: src/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=3 -o $@

$(OUTPUT_DIR)/win_cross_line: src/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=4 -o $@

$(OUTPUT_DIR)/win_hori_line2: src/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=5 -o $@

$(OUTPUT_DIR)/win_vert_line2: src/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=6 -o $@

$(OUTPUT_DIR)/win_cross_line2: src/win_test.cpp
	$(CC) $< $(COMPILER_FLAGS) -D TYPE=7 -o $@