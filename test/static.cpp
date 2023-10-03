#include <SDL3/SDL.h>
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

const int LINE_STEP = 100;
const int FRAME_SAMPLE = 60;

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
Uint32* data = NULL;
SDL_DisplayMode mode;
SDL_Rect rect;

std::vector<std::wstring> openWindow() {
    OPENFILENAME ofn;
    TCHAR szFile[260];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;  // If you have a parent window, set it here.
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

    if (GetOpenFileName(&ofn) == TRUE) {
        // The selected file path(s) are now stored in szFile.
        // Parse the paths and store them in a vector.
        std::vector<std::wstring> selectedFiles;
        TCHAR* pFile = szFile;
        while (*pFile) {
            int len = lstrlen(pFile);
            std::wstring filePath;
            if (len > 0) {
                int wideCharLen = MultiByteToWideChar(CP_ACP, 0, pFile, len, NULL, 0);
                if (wideCharLen > 0) {
                    std::vector<wchar_t> wideFilePath(wideCharLen + 1);
                    MultiByteToWideChar(CP_ACP, 0, pFile, len, &wideFilePath[0], wideCharLen);
                    wideFilePath[wideCharLen] = L'\0';
                    filePath = &wideFilePath[0];
                }
            }
            selectedFiles.push_back(filePath);
            pFile += len + 1;
        }

        return selectedFiles;
    } else {
        // If the user canceled the selection, return an empty vector.
        return std::vector<std::wstring>();
    }
}

int main() {
    std::vector<std::wstring> selectedFiles = openWindow();

    if (!selectedFiles.empty()) {
        // Process the selected files here.
        for (const std::wstring& filePath : selectedFiles) {
            MessageBox(NULL, filePath.c_str(), TEXT("Selected File"), MB_OK);
        }
    } else {
        MessageBox(NULL, TEXT("File selection canceled."), TEXT("Info"), MB_OK);
    }

    return 0;
}

void renderTexture(){
    SDL_UpdateTexture(texture, NULL, data, surface->pitch);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
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
    SDL_GetDesktopDisplayMode(1, & mode);
    std::cout << "Display mode of #" << count << " Display:\n\tw = " << mode.w
                << "\n\th = " << mode.h << "\n\trefreshrate = "
                << mode.refresh_rate << "\n\tpixel format = " 
                << SDL_GetPixelFormatName(mode.format) <<std::endl;

    //Get monitor bounds
    SDL_GetDisplayBounds(1, & rect);
    std::cout << "Display bound of #" << count << " Display:\n\tx = " << rect.x << "\n\ty = " << rect.y 
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

    //Create a texture for rendering context
    texture = SDL_CreateTexture(renderer, surface->format->format, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
    data = (Uint32*) calloc(rect.w * rect.h, unsigned(surface->format->BytesPerPixel));

    return 0;
}

int quitWindow(){
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit(); 
    return 0;
}

// int main(int argc, char* args[]){
    
//     std::vector<std::wstring> selectedFiles = openWindow();

//     if (!selectedFiles.empty()) {
//         // Process the selected files here.
//         for (const std::wstring& filePath : selectedFiles) {
//             MessageBox(NULL, filePath.c_str(), TEXT("Selected File"), MB_OK);
//         }
//     } else {
//         MessageBox(NULL, TEXT("File selection canceled."), TEXT("Info"), MB_OK);
//     }

//     initWindow();

//     SDL_Event e;
//     bool quit = false;
//     int iter = 0;
//     while (!quit){  
//         iter++;
//         SDL_PollEvent( &e );
//         if( e.type == SDL_QUIT ) 
//             quit = true;

//         //Update the surface
//         SDL_RenderPresent(renderer);
//     }
// 	quitWindow();

// 	return 0;
// }