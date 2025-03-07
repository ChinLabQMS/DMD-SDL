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

    window.selectAndLoadPatternMemory(NULL, true, true);
    window.displayPatternMemory(1000, true, true);
    
    std::cout << "Test completed, press any key to exit." << std::endl;
    std::cin.get();

    return 0;
}
