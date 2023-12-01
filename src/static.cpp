#include "SDL3/SDL.h"
#include "nfd.h"

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

const int TARGET_SCREEN_WIDTH = 912;
const int TARGET_SCREEN_HEIGHT = 1140;

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
    SDL_Rect rect;
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        //Get monitor bounds
        SDL_GetDisplayBounds(i, & rect);

        if (rect.w == TARGET_SCREEN_WIDTH && rect.h == TARGET_SCREEN_HEIGHT)
        {
            SDL_GetDesktopDisplayMode(i, & mode);
            std::cout << "Display mode of #" << i << " Display:\n\tw = " << mode.w
                << "\n\th = " << mode.h << "\n\trefreshrate = "
                << mode.refresh_rate << "\n\tpixel format = " 
                << SDL_GetPixelFormatName(mode.format) <<std::endl;
            
            found = true;
            break;
        }
    }
    
    if (!found)
    {
        std::cout << "No display with target resolution found!\n";
        return -1;
    }

    //Create a full screen window
    window = SDL_CreateWindow("Project window", 
                rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALWAYS_ON_TOP);
    if(!window){
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    
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