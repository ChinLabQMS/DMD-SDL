# DMD-SDL
Binary pattern streaming app developed with SDL3. Note that we are using a development version of SDL to utilize the high DPI windows support functionality. SDL3 is complied to a static library stored in [lib/libSDL3.a](lib/libSDL3.a).

Note that the code is specifically for Windows.

The code is developed to control Digital micromirror device (DMD, LightCrafter4500EVM from Texas Instrument) to realize real-time optical tweezer generation.
The binary patterns are packed to 24-bit RGB and sent to DMD via the parallel RGB interface, with DMD set to pattern sequence mode to project a designed sequence from a video source.

## static.cpp
Project a static bmp pattern to DMD. 
The code is dependent on [nativefiledialog](https://github.com/mlabbe/nativefiledialog.git) repo to generate a file selection window.
To use the code, first build the nfd to a static library and then copy it to the `lib` folder.