SRC_DIR = src
OUTPUT_DIR = bin

# Compiler settings
CC = g++
INCLUDE_PATHS = -I./include
LIBRARY_PATHS = -L./lib
CFLAGS_STANDARD = -Wall -O3 -fopenmp -DTEST=0
CFLAGS_TEST = -Wall -O3 -fopenmp -DTEST=1
LINKER_FLAGS = -lSDL3 -lm -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8

COMPILER_FLAGS_TEST = $(CFLAGS_TEST) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
COMPILER_FLAGS_STANDARD = $(CFLAGS_STANDARD) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

$(OUTPUT_DIR)/%.exe: $(SRC_DIR)/%.cpp $(SRC_DIR)/BaseWindow.cpp
	@echo "Building target: $@"
	$(CC) $^ -o $@ $(COMPILER_FLAGS_STANDARD)

$(OUTPUT_DIR)/%_novsync.exe: $(SRC_DIR)/%.cpp $(SRC_DIR)/BaseWindow.cpp
	@echo "Building target: $@"
	$(CC) $^ -o $@ $(COMPILER_FLAGS_TEST)

all: base_window

base_window: static black_white test_window

static: $(OUTPUT_DIR)/static.exe
black_white: $(OUTPUT_DIR)/black_white.exe
test_window: $(OUTPUT_DIR)/test_window.exe $(OUTPUT_DIR)/test_window_novsync.exe

test_pixel:
	$(CC) $(SRC_DIR)/test_pixel.cpp $(SRC_DIR)/PixelCanvas.cpp -o $(OUTPUT_DIR)/test_pixel.exe $(COMPILER_FLAGS_STANDARD)

clean:
	@echo "Cleaning up..."
	rm -f $(OUTPUT_DIR)/*.exe

.PHONY: all base_window static black_white test_window clean