#include <SDL3/SDL.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

const int LINE_STEP = 100;
const int FRAME_SAMPLE = 60;

// Set the initial directory (change this to your desired path)
const std::string INITIAL_DIR = std::filesystem::absolute("../resources").string();

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;

void openBmpFileDialog(std::vector<std::string>& selectedFiles)
{
    OPENFILENAMEA ofn; // Use ANSI version

    char szFile[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Bitmap Images (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrTitle = "Select .bmp Files";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
    ofn.lpstrInitialDir = INITIAL_DIR.c_str(); // Set initial directory

    if (GetOpenFileNameA(&ofn) == TRUE) // Use ANSI version
    {
        char* p = szFile;
        selectedFiles.push_back(p);

        while (*p != '\0')
        {
            p += strlen(p) + 1;
            if (*p != '\0')
            {
                selectedFiles.push_back(p);
            }
        }
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
                rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_FULLSCREEN);
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
    std::vector<std::string> selectedFiles;

    openBmpFileDialog(selectedFiles);
    
    initWindow();

    if (!selectedFiles.empty())
    {
        std::cout << "Selected files:" << std::endl;
        for (const std::string& file : selectedFiles)
        {
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