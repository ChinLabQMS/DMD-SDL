#include "SDL3/SDL.h"

const int TARGET_DISPLAY_WIDTH = 1080; //912;
const int TARGET_DISPLAY_HEIGHT = 1920; // 1140;

static const SDL_DialogFileFilter filters[] = {
    { "BMP images",  "bmp" },
    { "All files",   "*" }
};

static void SDLCALL callbackBMP(void* userdata, const char* const* filelist, int filter)
{
    if (!filelist) {
        SDL_Log("An error occured: %s", SDL_GetError());
        return;
    } else if (!*filelist) {
        SDL_Log("The user did not select any file.");
        return;
    }
    SDL_Log("Full path to selected file: '%s'", *filelist);
    SDL_Surface *bitmap_surface = SDL_LoadBMP(*filelist);
    if (!bitmap_surface) {
        SDL_Log("Unable to load bitmap: %s", SDL_GetError());
        return;
    }
    if (bitmap_surface->w != TARGET_DISPLAY_WIDTH || bitmap_surface->h != TARGET_DISPLAY_HEIGHT) {
        SDL_Log("Bitmap resolution does not match the target display resolution.");
        SDL_DestroySurface(bitmap_surface);
        return;
    }
    
    SDL_Window *window = (SDL_Window*) userdata;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bitmap_surface);
    SDL_DestroySurface(bitmap_surface);
    if (!texture) {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        return;
    }
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void initSDL()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: \t%s", SDL_GetError());
        exit(1);
    }
}

SDL_Window* initWindow()
{
    // Get the number of displays connected and find the target display
    int num_displays;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    SDL_Log("Number of display connected: %d, Primary display: #%d", num_displays, display);
    for (int i = 0; i < num_displays; i++) {
        const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(displays[i]);
        if (mode->w == TARGET_DISPLAY_WIDTH && mode->h == TARGET_DISPLAY_HEIGHT) {
            display = displays[i];
            SDL_Log("--->>>  Display with target resolution found: #%d   <<<---", display);
        }
        SDL_Log("Display mode of #%d Display:\n\tw = %d\n\th = %d\n\trefreshrate = %f\n\tpixel format = %s",
                mode->displayID, mode->w, mode->h, mode->refresh_rate, SDL_GetPixelFormatName(mode->format));
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
    SDL_Window *window = SDL_CreateWindowWithProperties(props);
    if (window) {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        SDL_Log("Window created successfully, width = %d, height = %d", w, h);
        SDL_HideCursor();
    }
    else {
        SDL_Log("Window could not be created: %s", SDL_GetError());
        exit(1);
    }

    // Clean up
    SDL_DestroyProperties(props);
    SDL_free(displays);
    return window;
}

int main(int argc, char* argv[])
{
    initSDL();
    SDL_Window *window = initWindow();

    void* userdata = window;
    SDL_ShowOpenFileDialog(callbackBMP, userdata, NULL, filters, 2, NULL, false);

    // Event loop
    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
