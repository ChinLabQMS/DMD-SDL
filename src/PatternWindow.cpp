#include <SDL3/SDL.h>
#include "PatternWindow.h"

PatternWindow::PatternWindow(int w, int h)
{
    window = NULL;
    renderer = NULL;
    texture = NULL;
    bitmap_surface = NULL;
    target_width = w;
    target_height = h;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: \t%s", SDL_GetError());
        exit(1);
    }
    display = SDL_GetPrimaryDisplay();

    // Get the number of displays connected and find the target display
    int num_displays;
    bool found = false;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
    SDL_Log("Number of display connected: %d, Primary display ID: #%d", num_displays, display);
    for (int i = 0; i < num_displays; i++) {
        const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(displays[i]);
        if (mode->w == target_width && mode->h == target_height) {
            display = displays[i];
            found = true;
            SDL_Log("--->>>  Display with target resolution (%d, %d) found: #%d   <<<---", 
                    target_width, target_height, display);
        }
        SDL_Log("Display mode of #%d Display:\n\tw = %d\n\th = %d\n\trefreshrate = %f\n\tpixel format = %s",
                mode->displayID, mode->w, mode->h, mode->refresh_rate, SDL_GetPixelFormatName(mode->format));
    }
    if (!found) {
        display = displays[num_displays - 1];
        SDL_Log("No display with target resolution (%d, %d) found. Use the last display.", 
                target_width, target_height);
    }
    SDL_free(displays);
}

PatternWindow::~PatternWindow()
{
    close();
    SDL_Log("Quitting SDL...");
    SDL_Quit();
}

void PatternWindow::init()
{
    if (window) {
        SDL_Log("Window already created.");
        return;
    }

    SDL_Rect rect;
    SDL_GetDisplayBounds(display, &rect);

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
        SDL_GetWindowSize(window, &window_width, &window_height);
        SDL_Log("Window created successfully, width = %d, height = %d.", window_width, window_height);
        SDL_HideCursor();

        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
        SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
        renderer = SDL_CreateRendererWithProperties(props);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Log("Renderer created successfully.");
        }
        else {
            SDL_Log("Renderer could not be created: %s", SDL_GetError());
        }
    }
    else {
        SDL_Log("Window could not be created: %s", SDL_GetError());
    }
}

void PatternWindow::close()
{
    if (window) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        window = NULL;
        renderer = NULL;
        SDL_Log("Window closed.");
    }
}

void PatternWindow::pause(int time_ms)
{
    SDL_Delay(time_ms);
}

// Load pattern from a BMP file to a bitmap surface
void PatternWindow::loadTexture(const char* filename)
{
    if (!window) {
        SDL_Log("Window not created, please initialize first.");
        return;
    }
    if (bitmap_surface) {
        SDL_DestroySurface(bitmap_surface);
        bitmap_surface = NULL;
    }
    bitmap_surface = SDL_LoadBMP(filename);
    if (bitmap_surface) {
        if (bitmap_surface->w != window_width || bitmap_surface->h != window_height) {
            SDL_Log("Pattern size (%d, %d) does not match window size (%d, %d)!",
                    bitmap_surface->w, bitmap_surface->h, window_width, window_height);
        }
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = NULL;
        }
        texture = SDL_CreateTextureFromSurface(renderer, bitmap_surface);
        if (texture) {
            SDL_Log("Texture created successfully");
        }
        else {
            SDL_Log("Texture could not be created: %s", SDL_GetError());
        }
    }
    else {
        SDL_Log("Surface could not be created: %s", SDL_GetError());
    }
}

// Project the texture to the window
void PatternWindow::projectTexture()
{
    if (!window) {
        SDL_Log("Window not created, please initialize first.");
        return;
    }
    if (!texture) {
        SDL_Log("Texture not created, please load a pattern first.");
        return;
    }
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

static const SDL_DialogFileFilter filters[] = {
    {"BMP images",  "bmp"},
    {"All files",   "*"}
};

static void SDLCALL callbackStaticFileSelect(void* userdata, const char* const* filelist, int filter)
{
    if (!filelist) {
        SDL_Log("An error occured during file selection: %s", SDL_GetError());
    } else if (!*filelist) {
        SDL_Log("The user did not select any file.");
    }
    else {
        SDL_Log("Full path to selected file: \n\t%s", *filelist);
        PatternWindow *window = (PatternWindow*) userdata;
        window->loadTexture(*filelist);
        window->projectTexture();
        SDL_Log("Pattern loaded and projected successfully.");
    }
}

// Select files from the file system
void PatternWindow::selectFileAndProject(const char* default_location)
{
    if (!window) {
        SDL_Log("Window not created, please initialize first.");
        return;
    }
    if (default_location) {
        SDL_Log("Default location (static pattern): %s", default_location);
    }
    else {
        SDL_Log("Default location (static pattern): NULL");
    }
    SDL_ShowOpenFileDialog(callbackStaticFileSelect, this, NULL, filters, 2, default_location, false);
}
