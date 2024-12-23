# DMD-SDL
Binary pattern streaming app developed with SDL3 to stream patterns displayed on a Digital micromirror device (DMD, LightCrafter4500EVM from Texas Instrument) to realize real-time optical tweezer generation.

The binary patterns are packed to 24-bit RGB and sent to DMD via the parallel RGB interface, with DMD set to pattern sequence mode to project a designed sequence from a video source.

## Setup
To use, download the source code or clone the repo with the following command:
```bash
git clone https://github.com/ChinLabQMS/DMD-SDL.git
```

## Dependencies

### SDL3 (version 3.1.6 pre-release)
We are using a pre-release version of [SDL](https://www.libsdl.org/) to utilize the high DPI windows support functionality.

#### Compiling SDL3
SDL3 is pre-complied to a static library stored in [lib/libSDL3.a](lib/libSDL3.a) for windows system.

You shouldn't have to compile SDL3 from source code, but if you want, you can follow the instructions here:

0. Make sure you have build tools that includes C++ compiler installed on your system. MinGW is recommended for Windows users.
1. Download the SDL3 source code from the SDL3 repository. The example here is for `SDL-preview-3.1.6` version. Newer versions of SDL3 may be available in the future with bug fixes and new features.
2. Download and install [CMake](https://cmake.org/). The default installation settings should expose the `cmake` command to the terminal.
3. Open a terminal and navigate to the SDL3 source code directory. Create a new directory (e.g. `SDL3_build`) to store the built libraries.
4. Run the following command to generate the build files, note that the path to the source code and the build directory may vary depending on your setup:
```bash
cmake -S ./SDL-preview-3.1.6 -B ./SDL3_build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DSDL_STATIC=ON
```
5. Navigate to the `SDL3_build` directory, it should now contains a `Makefile` that can be used with MinGW make tool. Run the following command to build the SDL3 libraries:
```bash
mingw32-make
```
6. The libraries should be built in the `SDL3_build/` directory. Default name should be `libSDL3.a`. You can copy this libraray file to the `lib` directory in the `DMD-SDL` project root directory.

## Create a window and render a static pattern
Create an opaque window with target size, and open a file selection dialog to select a BMP file, then render a static pattern loaded from BMP to the window.

To build an executable for `static.cpp`, run the following command in the project root directory:
```bash
mingw32-make static
```
It will build two executables: `static.exe` and `static_debug.exe` in the `bin` directory.
The `static_debug.exe` is built with a different default window size and is used for debugging purposes (e.g. to see the pattern on an actual monitor instead of DMD).
