#include "PatternWindow.h"
#include "Benchmark.h"

int main() {
    PatternWindow window;
    window.init(true);

    measureExecutionTime("Open window", &PatternWindow::open2, window, (std::string) "Diamond", true, true);
    measureExecutionTime("Set static pattern path", &PatternWindow::setStaticPatternPath2, window, (const char *) "../resources/text/RGB.bmp", true, true);
    SDL_Delay(5000);
    window.close(true);

    window.open2("Diamond", true, true);
    window.selectAndProject(NULL, true);
    SDL_Delay(5000);

    window.selectAndLoadPatternMemory(NULL, true, true);
    uint32_t indices[] = {0};
    window.displayPatternMemory(indices, 1, 0, true, true);
    
    double x0[] = {0, 1000, 1500};
    double y0[] = {0, 1000, 1500};
    window.projectBlackTweezerPattern(3, x0, y0, 100.0, 10.0, 10.0, 1, 100, 2000, true);
    
    std::cout << "Test completed, press any key to exit." << std::endl;
    std::cin.get();
    window.close(true);

    return 0;
}
