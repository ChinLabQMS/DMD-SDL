% Run this script to compile the mex files
curr_dir = string(pwd);
if curr_dir.endsWith("MATLAB-Andor")
    cd ../DMD-SDL
end
mex src/PatternWindowMex.cpp src/PatternWindow.cpp src/PixelCanvas.cpp src/BaseWindow.cpp -R2018a -output mex/PatternWindowMexTest CXXFLAGS="$CXXFLAGS -O3 -fopenmp -std=c++17" LDFLAGS="$LDFLAGS -fopenmp" -DTEST=0 -Iinclude -Llib -lSDL3 -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8
