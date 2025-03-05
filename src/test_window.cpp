#include "BaseWindow.h"
#include "Benchmark.h"

int main(int argc, char* argv[]){
    BaseWindow window;
    
    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Initialize SDL", &BaseWindow::init, window, true);
    measureExecutionTime("Open window", &BaseWindow::open, window, true);
    measureExecutionTime("Display color", &BaseWindow::displayColor, window, 255, 0, 0, true);
    measureExecutionTime("Read BMP", &BaseWindow::readBMP, window, (const char*) "../resources/text/CHIN.bmp", (SDL_Surface **) NULL, true);

    // Set static pattern
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/CHIN.bmp", false);
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/QMS.bmp", false);
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/RGB.bmp", false);
    SDL_Delay(2000);

    // Set dynamic pattern
    size_t pattern_size = window.getWindowWidth() * window.getWindowHeight();
    uint32_t** pattern = new uint32_t*[24];
    for (size_t i = 0; i < 23; i++) {
        pattern[i] = new uint32_t[pattern_size];
        uint32_t color = (~0U << (24 - (i + 1)));
        for (size_t j = 0; j < pattern_size; j++) {
            pattern[i][j] = color;
        }
    }
    pattern[23] = new uint32_t[pattern_size];
    for (size_t j = 0; j < pattern_size; j++) {
        pattern[23][j] = 0xFF000000;
    }
    pattern[23][0] = 0xFFFFFFFF;
    pattern[23][pattern_size - 1] = 0xFFFFFFFF;

    std::cout << "Testing dynamic pattern projection..." << std::endl;
    for (size_t i = 0; i < 23; i++) {
        measureExecutionTime("Set dynamic pattern (with parallel)", &BaseWindow::setDynamicPattern, window, (void *) pattern[i], false, true);
    }
    SDL_Delay(2000);
    std::cout << "Testing dynamic pattern projection..." << std::endl;
    for (size_t i = 0; i < 23; i++) {
        measureExecutionTime("Set dynamic pattern (with parallel)", &BaseWindow::setDynamicPattern, window, (void *) pattern[i], false, true);
    }
    SDL_Delay(2000);
    std::cout << "Testing dynamic pattern projection..." << std::endl;
    for (size_t i = 0; i < 23; i++) {
        measureExecutionTime("Set dynamic pattern", &BaseWindow::setDynamicPattern, window, (void *) pattern[i], false, false);
    }
    SDL_Delay(2000);
    std::cout << "Testing dynamic pattern projection..." << std::endl;
    for (size_t i = 0; i < 23; i++) {
        measureExecutionTime("Set dynamic pattern", &BaseWindow::setDynamicPattern, window, (void *) pattern[i], false, false);
    }
    SDL_Delay(2000);
    std::cout << "Testing dynamic pattern projection..." << std::endl;
    measureExecutionTime("Set dynamic pattern", &BaseWindow::setDynamicPattern, window, (void *) pattern[23], false, false);
    SDL_Delay(10000);

    window.selectAndProject();

    measureExecutionTime("Close window", &BaseWindow::close, window, true);

    std::cout << "Test complete." << std::endl;
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();

    // Free memory
    for (size_t i = 0; i < 24; i++) {
        delete[] pattern[i];
    }
    delete[] pattern;

    return 0;
}
