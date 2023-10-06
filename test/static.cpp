#include "SDL3/SDL.h"
#include "nfd.h"

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

const int LINE_STEP = 100;
const int FRAME_SAMPLE = 60;

// Set the initial directory (change this to your desired path)
const std::string INITIAL_DIR = std::filesystem::absolute("../resources").string();

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;

std::string openBmpFileDialog() {
    nfdchar_t* outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog("bmp", INITIAL_DIR.c_str(), &outPath);

    if (result == NFD_OKAY) {
        std::string bmpFilePath(outPath);
        free(outPath);
        return bmpFilePath;
    } else if (result == NFD_CANCEL) {
        std::cout << "File dialog canceled by the user." << std::endl;
        return "";
    } else {
        std::cerr << "File dialog error: " << NFD_GetError() << std::endl;
        return "";
    }
}

int initWindow(){
    // Set DPI awareness to avoid OS scaling
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitor");

	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

    int count = SDL_GetNumVideoDisplays();
    if (count < 1){
        std::cout << "No second display connected!\n";
        return -1;
    }
    else std::cout << "Number of displays connected: " << count << std::endl;

    //Get display mode
    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(count - 1, & mode);
    std::cout << "Display mode of #" << count - 1 << " Display:\n\tw = " << mode.w
                << "\n\th = " << mode.h << "\n\trefreshrate = "
                << mode.refresh_rate << "\n\tpixel format = " 
                << SDL_GetPixelFormatName(mode.format) <<std::endl;

    //Get monitor bounds
    SDL_Rect rect;
    SDL_GetDisplayBounds(count - 1, & rect);
    std::cout << "Display bound of #" << count - 1 << " Display:\n\tx = " << rect.x << "\n\ty = " << rect.y 
                << "\n\tw = " << rect.w << "\n\th = " << rect.h << std::endl;

    //Create a full screen window
    window = SDL_CreateWindow("Project window", 
                rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALWAYS_ON_TOP);
    if(!window){
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    //Get window surface
    surface = SDL_GetWindowSurface(window);
    std::cout << "Screen surface format:\n\tw = " << surface->w << "\n\th = " 
                << surface->h << "\n\tpixel format = " << SDL_GetPixelFormatName(surface->format->format) 
                << "\n\tbits per pixel = " << unsigned(surface->format->BitsPerPixel) << std::endl;
    
    //Hide cursor on window
    SDL_HideCursor();

    //Create a 2D rendering context for the surface
    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer){
        std::cout << "Unable to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    return 0;
}

int main()
{
    std::string file = openBmpFileDialog();
    
    initWindow();

    if (!file.empty())
    {
        std::cout << "Selected files:" << std::endl;
        std::cout << file << std::endl;
            
        SDL_Surface* image = SDL_LoadBMP(file.c_str());
        if (!image){
            std::cout << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
            return -1;
        }

        // Create a texture from the loaded image
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_DestroySurface(image);
        if (!texture) {
            std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
            return 1;
        }

        bool quit = false;
        SDL_Event event;

        while (!quit) {
            // Clear the renderer
            SDL_RenderClear(renderer);

            // Copy the texture to the renderer
            SDL_RenderTexture(renderer, texture, NULL, NULL);

            // Present the renderer
            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                }
            }
        }
        
        // Clean up and quit
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit(); 

    }
    else
    {
        std::cout << "No files selected." << std::endl;
    }

    return 0;
}