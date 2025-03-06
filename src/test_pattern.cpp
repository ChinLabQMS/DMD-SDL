#include "PatternWindow.h"
#include "Benchmark.h"

int main() {
    PatternWindow window;
    window.init(true);
    
    measureExecutionTime("Open window", &PatternWindow::open2, window, (std::string) "Diamond", true, true);
    measureExecutionTime("Set static pattern path", &PatternWindow::setStaticPatternPath2, window, (const char *) "../resources/text/RGB.bmp", true, true);
    measureExecutionTime("Close window", &PatternWindow::close, window, true);

    window.open2("Diamond", true, true);
    SDL_Delay(10000);
    window.selectAndProject(NULL, true);

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
    // for (size_t i = 0; i < 24; i++) {
    //     measureExecutionTime("loadPatternMemory", &PatternWindow::loadPatternMemory, window, (const uint32_t *) pattern[0], (size_t) pattern_size);
    // }
    
    std::cout << "Test completed, press any key to exit." << std::endl;
    std::cin.get();

    return 0;
}
