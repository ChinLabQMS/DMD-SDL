# DMD-SDL
Binary pattern streaming app developed with SDL3. Note that we are using a development version of SDL to utilize the high DPI windows support functionality. SDL3 is complied to a static library stored in [lib/libSDL3.a](lib/libSDL3.a).

Note that the code is specifically for Windows.

The code is developed to control Digital micromirror device (DMD, LightCrafter4500EVM from Texas Instrument) to realize real-time optical tweezer generation.
The binary patterns are packed to 24-bit RGB and sent to DMD via the parallel RGB interface, with DMD set to pattern sequence mode to project a designed sequence from a video source.

# Test code
The test code is stored in `test` folder under project directory.

## test.cpp
Generates several testing executables and save static patterns to bmp.
- gray.exe: Display gray full screen
- black.exe: Display gray full screen
- white.exe: Display white full screen
- red.exe: Display red full screen
- hori_line.exe: Display horizontal lines (1 px thick)
- vert_line.exe: Display vertical lines (1 px thick)
- hori_line2.exe: Display horizontal lines (2 px thick)
- vert_line2.exe: Display vertical lines (2 px thick)
- black_white.exe: Switching between black and white at screen's refresh rate

To build, use: 
```
mingw32-make test
```

## static.cpp
Project a static bmp pattern to DMD. 
The code is dependent on [nativefiledialog](https://github.com/mlabbe/nativefiledialog.git) repo to generate a file selection window.
To use the code, first build the nfd to a static library and then copy it to the `lib` folder.