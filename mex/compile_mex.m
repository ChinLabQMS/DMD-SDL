% Run this script to compile the mex files
mex src/PatternWindowMex.cpp src/PatternWindow.cpp -R2018a -output mex/PatternWindowMexTest -DTEST=1 -Iinclude -Llib -lSDL3 -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8
mex src/PatternWindowMex.cpp src/PatternWindow.cpp -R2018a -output mex/PatternWindowMex -DTEST=0 -Iinclude -Llib -lSDL3 -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8
