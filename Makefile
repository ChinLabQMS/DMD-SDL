#OBJS specifies which files to compile as part of the project
OBJS = win_test

#CC specifies which compiler we're using
CC = g++

#SOURCE_DIR specifies the source files directory
SOURCE_DIR = src

#OUTPUT_DIR specifies the output files directory
OUTPUT_DIR = bin

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I./include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L./lib

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS_DEBUG = -g -Wall -mwindows -mconsole
COMPILER_FLAGS_RELEASE = -w -Wl,-subsystem, windows -mwindows

#LINKER_FLAGS specifies the libraries we're linking against
# Need to link all dependencies for static library
STATIC_LIBS = -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8
LINKER_FLAGS = -lSDL3 $(STATIC_LIBS)

OBJS_NAME = $(patsubst %, $(OUTPUT_DIR)/%, $(OBJS))

#This is the target that compiles our executable
debug: COMPILER_FLAGS = $(COMPILER_FLAGS_DEBUG) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
debug: $(OBJS)

release: COMPILER_FLAGS = $(COMPILER_FLAGS_RELEASE) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
release: $(OBJS)

% : $(SOURCE_DIR)/%.cpp
	$(CC) $< $(COMPILER_FLAGS) -o $(OUTPUT_DIR)/$@
	$(OUTPUT_DIR)/$@