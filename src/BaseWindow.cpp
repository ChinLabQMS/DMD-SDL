#include <string>
#include "BaseWindow.h"

BaseWindow::~BaseWindow() {
    close();
    SDL_free(Displays);
    SDL_free(StaticPatternPath);
    SDL_free(BaseDirectory);
    Displays = NULL;
    StaticPatternPath = NULL;
    BaseDirectory = NULL;
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
            printf("No display with target resolution (%d, %d) found. Default to the last display, resolution (%d, %d).", 
                    TARGET_DISPLAY_WIDTH, TARGET_DISPLAY_HEIGHT, DisplayMode->w, DisplayMode->h);
    }
    // Get default path for opening file dialog
    BaseDirectory = SDL_GetCurrentDirectory();
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
            printf("Window already created.");
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

        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, Window);
        SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
        Renderer = SDL_CreateRendererWithProperties(props);
        SDL_DestroyProperties(props);
        if (Renderer) {
            if (!StaticPatternPath) {
                displayColor(0, 0, 0);
            } else {
                setStaticPatternPath(StaticPatternPath, verbose);
            }
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
        SDL_DestroySurface(StaticPatternSurface);
        SDL_DestroySurface(DynamicPatternSurface);
        Window = NULL;
        Renderer = NULL;
        StaticPatternSurface = NULL;
        DynamicPatternSurface = NULL;
        if (verbose) {
            printf("Window closed.");}
    }
}

bool BaseWindow::isWindowCreated() {
    return Window != NULL;
}

bool BaseWindow::isWindowMinimized() {
    if (!Window) {
        return false;
    }
    return SDL_GetWindowFlags(Window) & SDL_WINDOW_MINIMIZED;
}

// Display a solid color on the window
void BaseWindow::displayColor(int r, int g, int b, bool verbose) {
    if (!Window) {
        open(verbose);
    }
    SDL_SetRenderDrawColor(Renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Renderer);
    SDL_RenderPresent(Renderer);
    SDL_free(OperationMode);
    OperationMode = (char*) SDL_malloc(256);
    snprintf(OperationMode, 256, "Color");
}

// Load pattern from a BMP file and display it on the window
void BaseWindow::setStaticPatternPath(const char* filepath, 
                                      bool verbose) {
    if (!filepath) {
        return;
    }
    SDL_free(StaticPatternPath);
    StaticPatternPath = (char*) SDL_malloc(256);
    snprintf(StaticPatternPath, 256, "%s", filepath);
    if (!Window) {
        return;
    }
    SDL_Surface *bitmap_surface = SDL_LoadBMP(StaticPatternPath);
    if (bitmap_surface) {
        if ((bitmap_surface->w != WindowWidth || bitmap_surface->h != WindowHeight) && (verbose)) {
            printf("Pattern size (%d, %d) does not match window size (%d, %d)!",
                    bitmap_surface->h, bitmap_surface->w, WindowWidth, WindowHeight);
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer, bitmap_surface);
        SDL_RenderClear(Renderer);
        SDL_RenderTexture(Renderer, texture, NULL, NULL);
        SDL_DestroyTexture(texture);
        // Read the pixels from the renderer instead of using the bitmap_surface
        // to ensure the correct surface size
        SDL_DestroySurface(bitmap_surface);
        bitmap_surface = SDL_RenderReadPixels(Renderer, NULL);
        SDL_DestroySurface(StaticPatternSurface);
        StaticPatternSurface = SDL_ConvertSurface(bitmap_surface, SDL_PIXELFORMAT_ARGB8888);
        SDL_DestroySurface(bitmap_surface);
        SDL_RenderPresent(Renderer);
        SDL_free(OperationMode);
        OperationMode = (char*) SDL_malloc(256);
        snprintf(OperationMode, 256, "Static");
        if (verbose)
            printf("Pattern projected successfully from path:\n\t%s.", StaticPatternPath);
    }
    else
        printf("Surface could not be created from BMP file: %s", SDL_GetError());
}


// Project a pattern on the window with a pointer to the pixel data
void BaseWindow::setDynamicPattern(void* pattern, bool verbose) {
    if (!Window) {
        open(verbose);
    }
    SDL_DestroySurface(DynamicPatternSurface);
    // Pixel data is assumed to be in ARGB format, pitch is 4 * WindowWidth
    DynamicPatternSurface = SDL_CreateSurfaceFrom(WindowWidth, WindowHeight, SDL_PIXELFORMAT_ARGB8888, pattern, 4 * WindowWidth);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer, DynamicPatternSurface);    
    SDL_RenderClear(Renderer);
    SDL_RenderTexture(Renderer, texture, NULL, NULL);
    SDL_RenderPresent(Renderer);
    SDL_DestroyTexture(texture);
    if (verbose) {
        printf("Pattern projected successfully.");
    }
    SDL_free(OperationMode);
    OperationMode = (char*) SDL_malloc(256);
    snprintf(OperationMode, 256, "Dynamic");
}

const SDL_DialogFileFilter filters[] = {
    {"BMP images",  "bmp"},
    {"All files",   "*"}
};

void SDLCALL callbackStaticFileSelect(void* userdata, const char* const* filelist, int filter) {
    BaseWindow *window = (BaseWindow*) userdata;
    if (!filelist)
        window->error("An error occured during file selection: %s", SDL_GetError());
    else if (!*filelist)
        window->error("The user did not select any file.");
    else {
        window->setStaticPatternPath(*filelist);
    }
}

// Select files from the file system
void BaseWindow::selectAndProject(const char* default_location, bool verbose) {
    if (!default_location) {
        default_location = BaseDirectory;
    }
    if (verbose) {
        if (default_location)
            printf("Default location (static pattern): %s", default_location);
        else
            printf("Default location (static pattern): NULL");
    }
    SDL_ShowOpenFileDialog(callbackStaticFileSelect, this, NULL, filters, 2, default_location, false);
}

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

int BaseWindow::getDisplayIndex() {
    return DisplayIndex;
}

int BaseWindow::getWindowHeight() {
    if (Window)
        return WindowHeight;
    else
        return 0;
}

int BaseWindow::getWindowWidth() {
    if (Window)
        return WindowWidth;
    else
        return 0;
}

const char* BaseWindow::getOperationMode() {
    return OperationMode;
}

const char* BaseWindow::getStaticPatternPath() {
    return StaticPatternPath;
}

const char* BaseWindow::getBaseDirectory() {
    return BaseDirectory;
}

SDL_Surface* BaseWindow::getStaticPatternSurface() {
    return StaticPatternSurface;
}

SDL_Surface* BaseWindow::getDynamicPatternSurface() {
    return DynamicPatternSurface;
}
