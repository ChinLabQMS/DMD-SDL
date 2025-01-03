#include <cstdio>
#include "PatternWindow.h"

BaseWindow::BaseWindow() {
    window = NULL;
    renderer = NULL;
    display_mode = NULL;
    displays = NULL;
    target_width = TARGET_DISPLAY_WIDTH;
    target_height = TARGET_DISPLAY_HEIGHT;
}

BaseWindow::~BaseWindow() {
    close();
    SDL_free(displays);
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

void BaseWindow::init(bool verbose) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        error("Unable to initialize SDL: \t%s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    else if (verbose)
        printf("SDL initialized successfully.");

    // Get the number of displays connected and find the target display
    bool found = false;
    display_id = SDL_GetPrimaryDisplay();
    displays = SDL_GetDisplays(&num_displays);
    if (verbose)
        printf("Number of display connected: %d, Primary display ID: #%d", num_displays, display_id);
    if (num_displays == 0) {
        SDL_Quit();
        exit(2);
    }
    for (int i = 0; i < num_displays; i++) {
        const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(displays[i]);
        if (mode->w == target_width && mode->h == target_height) {
            display_index = i;
            display_id = displays[i];
            display_mode = (SDL_DisplayMode*) mode;
            found = true;
            if (verbose) {
                if (i == 0)
                    printf("Display (primary) with target resolution (%d, %d) found, display ID: #%d.", 
                            target_width, target_height, display_id);
                else
                    printf("Display with target resolution (%d, %d) found, display ID: #%d.", 
                            target_width, target_height, display_id);
            }
        }
    }
    if (!found) {
        display_index = num_displays - 1;
        display_id = displays[num_displays - 1];
        display_mode = (SDL_DisplayMode*) SDL_GetDesktopDisplayMode(display_id);
        if (verbose)
            printf("No display with target resolution (%d, %d) found. Default to the last display, resolution (%d, %d).", 
                    target_width, target_height, display_mode->w, display_mode->h);
    }
}

void BaseWindow::setDisplayIndex(int idx, bool verbose) {
    if (window) {
        if (verbose)
            printf("Window already created, can not set the display index.");
        return;
    }
    if ((idx >= 0) & (idx < num_displays) & (idx != display_index)) {
        display_index = idx;
        display_id = displays[idx];
        display_mode = (SDL_DisplayMode*) SDL_GetDesktopDisplayMode(display_id);
        if (verbose)
            printf("Set displayID to #%d, resolution: (%d, %d), refresh rate: %f, pixel format: %s",
                display_id, display_mode->w, display_mode->h, display_mode->refresh_rate, SDL_GetPixelFormatName(display_mode->format));
    }
}

int BaseWindow::getDisplayIndex() {
    return display_index;}

void BaseWindow::open(int idx, bool verbose) {
    if (window) {
        if (verbose)
            printf("Window already created.");
        return;
    }
    setDisplayIndex(idx);

    SDL_Rect rect;
    SDL_GetDisplayBounds(display_id, &rect);

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
    window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);
    if (window) {
        SDL_SetWindowFullscreenMode(window, NULL);
        SDL_SetWindowFullscreen(window, true);
        SDL_SyncWindow(window);
        SDL_RaiseWindow(window);

        SDL_GetWindowSize(window, &window_width, &window_height);
        if (verbose)
            printf("Window created successfully, resolution: (%d, %d).", window_width, window_height);
        SDL_HideCursor();

        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
        SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
        renderer = SDL_CreateRendererWithProperties(props);
        SDL_DestroyProperties(props);
        if (renderer) {
            displayColor(0, 0, 0);
        }
        else {
            error("Renderer could not be created: %s", SDL_GetError());
        }
    }
    else {
        error("Window could not be created: %s", SDL_GetError());
    }
}

void BaseWindow::close(bool verbose) {
    if (window) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        window = NULL;
        renderer = NULL;
        if (verbose)
            printf("Window closed.");
    }
}

bool BaseWindow::isWindowCreated() {
    return window != NULL;
}

void BaseWindow::displayColor(int r, int g, int b) {
    if (!window) {
        open(-1, false);
        return;
    }
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

// Load pattern from a BMP file to a texture
void BaseWindow::projectFromFile(const char* filename, 
                                int idx, 
                                bool verbose) {
    if (!window) {
        open(idx, verbose);
        return;
    }
    SDL_Surface *bitmap_surface = SDL_LoadBMP(filename);
    if (bitmap_surface) {
        if (bitmap_surface->w != window_width || bitmap_surface->h != window_height)
            if (verbose)
                printf("Pattern size (%d, %d) does not match window size (%d, %d)!",
                        bitmap_surface->w, bitmap_surface->h, window_width, window_height);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bitmap_surface);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(bitmap_surface);
        if (verbose)
            printf("Pattern projected successfully from path:\n\t%s.", filename);
    }
    else
        printf("Surface could not be created from BMP file: %s", SDL_GetError());
}

static const SDL_DialogFileFilter filters[] = {
    {"BMP images",  "bmp"},
    {"All files",   "*"}
};

static void SDLCALL callbackStaticFileSelect(void* userdata, const char* const* filelist, int filter) {
    PatternWindow *window = (PatternWindow*) userdata;
    if (!filelist)
        window->error("An error occured during file selection: %s", SDL_GetError());
    else if (!*filelist)
        window->error("The user did not select any file.");
    else {
        window->projectFromFile(*filelist);
    }
}

// Select files from the file system
void BaseWindow::selectAndProject(const char* default_location, bool verbose) {
    if (!window) {
        open(-1, verbose);
        return;
    }
    if (verbose) {
        if (default_location)
            printf("Default location (static pattern): %s", default_location);
        else
            printf("Default location (static pattern): NULL");
    }
    SDL_ShowOpenFileDialog(callbackStaticFileSelect, this, NULL, filters, 2, default_location, false);
}

PatternWindow::PatternWindow(bool verbose) {
    init(verbose);
}

PatternWindow::~PatternWindow() {
    printf("Quitting SDL...");
}
