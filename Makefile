#OBJS specifies which files to compile as part of the project
OBJS = main

#OUTPUT specifies the name of our exectuable
OUTPUT = bin/$(OBJS)

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -Iinclude

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -Llib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w -Wl,-subsystem, windows
# -w suppresses all warnings
# -Wl,-subsystem, windows gets rid of the console window
COMPILER_FLAGS = -g -Wall

#LINKER_FLAGS specifies the libraries we're linking against 
# -lmingw32 -lSDL2 -lSDL2main
LINKER_FLAGS = -lmingw32 -lSDL3

#This is the target that compiles our executable
all : 
	$(CC) $(OBJS).cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT)
	$(OUTPUT)