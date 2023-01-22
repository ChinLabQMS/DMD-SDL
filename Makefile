SOURCE = win_test
OUTPUT = win_gray win_vert_line win_hori_line

SOURCE_DIR = src
SOURCE_NAME = $(SOURCE_DIR)/$(SOURCE).cpp
OUTPUT_DIR = bin
OUTPUT_NAME = $(patsubst %, $(OUTPUT_DIR)/%, $(OUTPUT))

CC = g++
INCLUDE_PATHS = -I./include
LIBRARY_PATHS = -L./lib
COMPILER_FLAGS_DEBUG = -g -Wall -mwindows -mconsole
COMPILER_FLAGS_RELEASE = -w -Wl,-subsystem, windows -mwindows
LINKER_FLAGS = -lSDL3 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8

debug: COMPILER_FLAGS = $(COMPILER_FLAGS_DEBUG) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
debug: $(OUTPUT)

release: COMPILER_FLAGS = $(COMPILER_FLAGS_RELEASE) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
release: $(OUTPUT)

win_gray:
	$(CC) $(SOURCE_NAME) $(COMPILER_FLAGS) -D TYPE=1 -o $(OUTPUT_DIR)/$@
	$(OUTPUT_DIR)/$@

win_hori_line:
	$(CC) $(SOURCE_NAME) $(COMPILER_FLAGS) -D TYPE=2 -o $(OUTPUT_DIR)/$@
	$(OUTPUT_DIR)/$@

win_vert_line:
	$(CC) $(SOURCE_NAME) $(COMPILER_FLAGS) -D TYPE=3 -o $(OUTPUT_DIR)/$@
	$(OUTPUT_DIR)/$@