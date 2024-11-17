# DMD-SDL
Binary pattern streaming app developed with SDL3 to stream patterns displayed on a Digital micromirror device (DMD, LightCrafter4500EVM from Texas Instrument) to realize real-time optical tweezer generation.

The binary patterns are packed to 24-bit RGB and sent to DMD via the parallel RGB interface, with DMD set to pattern sequence mode to project a designed sequence from a video source.

## Dependencies

### SDL (version 3.1.6 pre-release)
We are using a pre-release version of SDL to utilize the high DPI windows support functionality.
The official website for SDL is [here](https://www.libsdl.org/).

#### Compiling SDL3
SDL3 is pre-complied to a static library stored in <!-- Need to fill -->
[]() for windows system.

You shouldn't need to compile SDL3 yourself, but if you do, you can follow the instructions here:
0. Make sure you have a C++ compiler installed on your system. MinGW is recommended for Windows users.
1. Download the SDL3 source code from the SDL3 repository. The example here is for `SDL-preview-3.1.6` version. Newer versions of SDL3 may be available in the future with bug fixes and new features.
2. Download and install CMake from the CMake website. The default installation settings should expose the `cmake` command to the terminal.
3. Open a terminal and navigate to the SDL3 source code directory. Create a new directory called `SDL3_build` to store the built libraries.
4. Run the following command to generate the build files, note that the path to the SDL3 source code and the SDL3_build directory may vary depending on your system.:
```
cmake -S ./SDL-preview-3.1.6 -B ./SDL3_build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DSDL_STATIC=ON
```
5. Navigate to the `SDL3_build` directory and run the following command to build the SDL3 libraries:
```
mingw32-make
```
6. The libraries should be built in the `SDL3_build/` directory. Default name should be `libSDL3.a`. You can copy this libraray file to the `lib` directory in the `DMD-SDL` project root directory.

## Building the project

### static.cpp
Project a static bmp pattern to DMD. 
