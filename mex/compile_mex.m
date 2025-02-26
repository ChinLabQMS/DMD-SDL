% Run this script to compile the mex files
curr_dir = string(pwd);
if curr_dir.endsWith("MATLAB-Andor")
    cd ../DMD-SDL
end
mex src/PatternWindowMex.cpp src/PixelCanvas.cpp src/PatternWindow.cpp -R2018a -output mex/PatternWindowMexTest CXXFLAGS="$CXXFLAGS -O3 -fopenmp" LDFLAGS="$LDFLAGS -fopenmp" -DTEST=1 -Iinclude -Llib -lSDL3 -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8
mex src/PatternWindowMex.cpp src/PixelCanvas.cpp src/PatternWindow.cpp -R2018a -output mex/PatternWindowMex CXXFLAGS="$CXXFLAGS -O3 -fopenmp" LDFLAGS="$LDFLAGS -fopenmp" -DTEST=0 -Iinclude -Llib -lSDL3 -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8
