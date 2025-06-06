#include <string>
#include "BaseWindow.h"

void replaceBackslash(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == '\\') {
            str[i] = '/';
        }
    }
}

BaseWindow::~BaseWindow() {
    close(false);
    SDL_free(Displays);
    SDL_free(StaticPatternPath);
    SDL_free(BaseDirectory);
    SDL_DestroySurface(BMPSurface);
    Displays = NULL;
    StaticPatternPath = NULL;
    BaseDirectory = NULL;
    BMPSurface = NULL;
    SDL_Quit();
}

void BaseWindow::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);
    va_end(args);
}

void BaseWindow::error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, format, args);
    va_end(args);
}

void BaseWindow::warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, format, args);
    va_end(args);
}

// Initialize SDL and find the target display
// If verbose is true, print messages to the console
void BaseWindow::init(bool verbose) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        error("Unable to initialize SDL: \t%s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    // Get the number of displays connected and find the target display
    bool found = false;
    DisplayID = SDL_GetPrimaryDisplay();
    Displays = SDL_GetDisplays(&NumDisplays);
    if (verbose)
        printf("Number of display connected: %d", NumDisplays);
    if (NumDisplays == 0) {
        SDL_Quit();
        exit(2);
    }
    for (int i = 0; i < NumDisplays; i++) {
        const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(Displays[i]);
        if (mode->w == TARGET_DISPLAY_WIDTH && mode->h == TARGET_DISPLAY_HEIGHT) {
            DisplayIndex = i;
            DisplayID = Displays[i];
            DisplayMode = (SDL_DisplayMode*) mode;
            found = true;
            if (verbose) {
                if (i == 0)
                    printf("Display (primary) with target resolution (%d, %d) found.", 
                            TARGET_DISPLAY_WIDTH, TARGET_DISPLAY_HEIGHT);
                else
                    printf("Display (non-primary) with target resolution (%d, %d) found, display index (0-start): #%d.",
                            TARGET_DISPLAY_WIDTH, TARGET_DISPLAY_HEIGHT, DisplayIndex);
            }
        }
    }
    if (!found) {
        DisplayIndex = NumDisplays - 1;
        DisplayID = Displays[NumDisplays - 1];
        DisplayMode = (SDL_DisplayMode*) SDL_GetDesktopDisplayMode(DisplayID);
        if (verbose)
            warn("No display with target resolution (%d, %d) found. Default to the last display, resolution (%d, %d).", 
                    TARGET_DISPLAY_WIDTH, TARGET_DISPLAY_HEIGHT, DisplayMode->w, DisplayMode->h);
    }
    // Get default path for opening file dialog, reformat to remove backslashes
    BaseDirectory = SDL_GetCurrentDirectory();
    replaceBackslash(BaseDirectory);
    if (verbose)
        printf("Base directory: %s", BaseDirectory);
}

// Create a window on the target display
// The window is borderless, always on top, and high pixel density, with the same resolution as the display
// If StaticPatternPath is not NULL, load the pattern from the file and display it on the window
// If verbose is true, print messages to the console
void BaseWindow::open(bool verbose) {
    if (Window) {
        if (verbose)
            warn("Window already created.");
        return;
    }    
    SDL_Rect rect;
    SDL_GetDisplayBounds(DisplayID, &rect);
    // Create a window on the target display
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Pattern window");
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, rect.x);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, rect.y);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, rect.w);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, rect.h);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true);
    Window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);
    if (Window) {
        SDL_SetWindowFullscreenMode(Window, NULL);
        SDL_SetWindowFullscreen(Window, true);
        SDL_SyncWindow(Window);
        SDL_RaiseWindow(Window);

        SDL_GetWindowSize(Window, &WindowWidth, &WindowHeight);
        if (verbose)
            printf("Window created successfully, resolution: (%d, %d).", WindowHeight, WindowWidth);
        SDL_HideCursor();
        // Create a renderer for the window
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, Window);
        SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, RENDERER_VSYNC);
        Renderer = SDL_CreateRendererWithProperties(props);
        SDL_DestroyProperties(props);
        if (Renderer) {
            // Create a streaming texture for dynamic pattern projection
            Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WindowWidth, WindowHeight);
            displayColor(0, 0, 0, verbose);
        } else {
            error("Renderer could not be created: %s", SDL_GetError());}
    } else {
        error("Window could not be created: %s", SDL_GetError());}
}

// Close the window and free resources
void BaseWindow::close(bool verbose) {
    if (Window) {
        SDL_DestroyRenderer(Renderer);
        SDL_DestroyWindow(Window);
        SDL_DestroyTexture(Texture);
        SDL_DestroySurface(StaticPatternSurface);
        Window = NULL;
        Renderer = NULL;
        Texture = NULL;
        StaticPatternSurface = NULL;
        WindowWidth = 0;
        WindowHeight = 0;
        if (verbose) {
            printf("Window closed.");}
    }
}

// Display a solid color on the window
void BaseWindow::displayColor(int r, int g, int b, bool verbose) {
    if (!Window) {
        error("Window not created, cannot display color.");
        return;
    }
    SDL_SetRenderDrawColor(Renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Renderer);
    SDL_RenderPresent(Renderer);
    SDL_free(OperationMode);
    OperationMode = SDL_strdup("Color");
    ColorModeR = r;
    ColorModeG = g;
    ColorModeB = b;
    if (verbose) {
        printf("Color displayed successfully: (%d, %d, %d).", r, g, b);
    }
}

// Read a BMP file and create a surface from it
void BaseWindow::readBMP(const char* filename, SDL_Surface **surface, bool verbose) {
    SDL_Surface *bitmap_surface = SDL_LoadBMP(filename);
    if (bitmap_surface) {
        if (surface) {
            SDL_DestroySurface(*surface);
            *surface = SDL_ConvertSurface(bitmap_surface, SDL_PIXELFORMAT_ARGB8888);
        }
        else {
            SDL_DestroySurface(BMPSurface);
            BMPSurface = SDL_ConvertSurface(bitmap_surface, SDL_PIXELFORMAT_ARGB8888);
        }
        SDL_DestroySurface(bitmap_surface);
        SDL_free(BMPPath);
        BMPPath = SDL_strdup(filename);
        replaceBackslash(BMPPath);
        if (verbose) {
            printf("BMP file loaded successfully from path:\n\t%s", BMPPath);
        }
    } else {
        error("Surface could not be created from BMP file: %s", SDL_GetError());
    }
}

// Load pattern from a BMP file and display it on the window
void BaseWindow::setStaticPatternPath(const char* filepath, 
                                      bool verbose) {
    if (!filepath) {
        return;
    }
    SDL_free(StaticPatternPath);
    StaticPatternPath = SDL_strdup(filepath);
    replaceBackslash(StaticPatternPath);
    // If the window is not created, return
    if (!Window) {
        return;
    }
    SDL_Surface *bitmap_surface = SDL_LoadBMP(StaticPatternPath);
    if (bitmap_surface) {
        if ((bitmap_surface->w != WindowWidth || bitmap_surface->h != WindowHeight) && (verbose)) {
            warn("Pattern size (%d, %d) does not match window size (%d, %d)! Pattern will be resized to fit the window.",
                    bitmap_surface->h, bitmap_surface->w, WindowWidth, WindowHeight);
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer, bitmap_surface);
        SDL_RenderClear(Renderer);
        SDL_RenderTexture(Renderer, texture, NULL, NULL);
        SDL_DestroySurface(bitmap_surface);
        SDL_DestroyTexture(texture);
        // Read the pixels from the renderer instead of using the bitmap_surface
        // to ensure the correct surface size (SLOW!)
        SDL_DestroySurface(StaticPatternSurface);
        bitmap_surface = SDL_RenderReadPixels(Renderer, NULL);
        StaticPatternSurface = SDL_ConvertSurface(bitmap_surface, SDL_PIXELFORMAT_ARGB8888);
        SDL_DestroySurface(bitmap_surface);
        SDL_RenderPresent(Renderer);
        SDL_free(OperationMode);
        OperationMode = SDL_strdup("Static");
        if (verbose)
            printf("Pattern projected successfully from path:\n\t%s.", StaticPatternPath);
    }
    else {
        printf("Surface could not be created from BMP file: %s", SDL_GetError());
    }
}


// Project a pattern on the window with a pointer to the pixel data
void BaseWindow::setDynamicPattern(void* pattern, bool verbose, bool use_parallel) {
    if (!Window) {
        open(verbose);
    }
    // Pixel data is assumed to be in ARGB format, pitch is 4 * WindowWidth + padding
    void *pixels;
    int pitch;
    SDL_LockTexture(Texture, NULL, &pixels, &pitch);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < WindowHeight; i++) {
        memcpy((uint8_t*) pixels + i * pitch, 
               (uint8_t*) pattern + 4 * i * WindowWidth, 
               sizeof(uint32_t) * WindowWidth);
    }
    SDL_UnlockTexture(Texture);
    SDL_RenderClear(Renderer);
    SDL_RenderTexture(Renderer, Texture, NULL, NULL);
    SDL_RenderPresent(Renderer);
    if (verbose) {
        printf("Dynamic pattern projected successfully.");
    }
    SDL_free(OperationMode);
    OperationMode = SDL_strdup("Dynamic");
}

// Callback function for file selection dialog
void SDLCALL callback(void* userdata, const char* const* filelist, int filter) {
    SDL_Event user_event;
    SDL_zero(user_event);
    user_event.type = SDL_EVENT_USER;
    user_event.user.data1 = SDL_malloc(sizeof(int)); // Number of files selected
    user_event.user.data2 = NULL; // File path
    if (!filelist) {
        user_event.user.code = -1;
    } else if (!*filelist) {
        user_event.user.code = 1;
    } else {
        user_event.user.code = 0;
        int count = 0;
        while (filelist[count]) {
            count++;            
        }
        user_event.user.data2 = (char**) (SDL_malloc(count * sizeof(char*)));
        for (int i = 0; i < count; i++) {
            ((char**) user_event.user.data2)[i] = SDL_strdup(filelist[i]);
            replaceBackslash(((char**) user_event.user.data2)[i]);
        }
        *((int*) user_event.user.data1) = count;
    }
    SDL_PushEvent(&user_event);
}

// Select files from the file system and project on the window
void BaseWindow::selectAndProject(const char* default_location, bool verbose) {
    if (!default_location) {
        default_location = BaseDirectory;
    }
    SDL_ShowOpenFileDialog(callback, NULL, NULL, filters, 2, default_location, false);
    // Delay the return until the user input is received
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_USER) {
                if (event.user.code == -1) {
                    error("An error occured during file selection: %s", SDL_GetError());
                }
                if (event.user.code == 1) {
                    warn("The user did not select any file.");
                }
                if (event.user.code == 0) {
                    setStaticPatternPath(((char**) event.user.data2)[0], verbose);
                }
                return;
            }
        }
    }
}

// Set the display index to the specified value
// If the index is out of range, do nothing
void BaseWindow::setDisplayIndex(int idx, bool verbose) {
    if ((idx >= 0) & (idx < NumDisplays) & (idx != DisplayIndex)) {
        DisplayIndex = idx;
        DisplayID = Displays[idx];
        DisplayMode = (SDL_DisplayMode*) SDL_GetDesktopDisplayMode(DisplayID);
        if (verbose)
            printf("Set display index to %d, resolution: (%d, %d), refresh rate: %f, pixel format: %s",
                DisplayIndex, DisplayMode->w, DisplayMode->h, DisplayMode->refresh_rate, 
                SDL_GetPixelFormatName(DisplayMode->format));
        if (Window) {
            close(verbose);
            open(verbose);
        }
    }
}

// Check if the window is created
bool BaseWindow::isWindowCreated() const {
    return Window != NULL;
}

// Check if the window is minimized
// If the window is not created, return false
bool BaseWindow::isWindowMinimized() const {
    if (!Window) {
        return false;
    }
    return SDL_GetWindowFlags(Window) & SDL_WINDOW_MINIMIZED;
}

// Get the width of the window
int BaseWindow::getWindowWidth() const {
    return WindowWidth;
}

// Get the height of the window
int BaseWindow::getWindowHeight() const {
    return WindowHeight;
}

// Check the state of the window
// If the window is not created, print an error message
// If the window is minimized, print a warning message
// If the window is created and not minimized, do nothing
void BaseWindow::checkWindowState() {
    if (!isWindowCreated()) {
        error("Window is not created.");
    } else if (isWindowMinimized()) {
        error("Window is minimized.");
    }
}

// Save pixel data to a BMP file
// The pixel data is assumed to be in ARGB format, pitch is 4 * width + padding
// If verbose is true, print messages to the console
// The filename is the full path to the file, including the file name and extension
void BaseWindow::savePixelsAsBMP(const char* filename, void *pixels, int width, int height, int pitch, bool verbose) {
    SDL_Surface *surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_ARGB8888, 
        pixels, pitch);
    SDL_SaveBMP(surface, filename);
    SDL_DestroySurface(surface);
    if (verbose) {
        printf("Pixels data saved to BMP file: %s", filename);
    }
}

// Select a file from the file system and save pixel data to it
// The pixel data is assumed to be in ARGB format, pitch is 4 * width + padding
// If verbose is true, print messages to the console
// The filename is the full path to the file, including the file name and extension
void BaseWindow::selectAndSavePixelsAsBMP(const char* default_location, void *pixels, int width, int height, int pitch, bool verbose) {
    if (!default_location) {
        default_location = BaseDirectory;
    }
    SDL_ShowSaveFileDialog(callback, NULL, NULL, filters, 2, default_location);
    // Delay the return until the user input is received
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_USER) {
                if (event.user.code == -1) {
                    error("An error occured during file selection: %s", SDL_GetError());
                }
                if (event.user.code == 1) {
                    warn("The user did not select any file.");
                }
                if (event.user.code == 0) {
                    savePixelsAsBMP(((char**) event.user.data2)[0], pixels, width, height, pitch, verbose);
                }
                return;
            }
        }
    }
}
