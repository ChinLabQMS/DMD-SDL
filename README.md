# DMD-SDL
Binary pattern streaming app developed with SDL3. Note that we are using a development version of SDL to utilize the high DPI windows support functionality. SDL3 is complied to a static library.

The code will be used to control Digital micromirror device (DMD, LightCrafter4500EVM from Texas Instrument) to realize real-time optical tweezer generation.

# Test code
## win_test.cpp
Generates several testing executables and save static patterns to bmp.
- win_gray.exe: Display gray full screen
- win_black.exe: Display gray full screen
- win_white.exe: Display white full screen
- win_red.exe: Display red full screen
- win_hori_line.exe: Display horizontal lines (1 px thick)
- win_vert_line.exe: Display vertical lines (1 px thick)
- win_hori_line2.exe: Display horizontal lines (2 px thick)
- win_vert_line2.exe: Display vertical lines (2 px thick)
- win_black_white.exe: Switching between black and white at screen's refresh rate

To build, use: 
```
mingw32-make test
```

# Source code
## win_MATLAB.cpp
