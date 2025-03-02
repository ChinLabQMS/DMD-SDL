#include "PixelCanvas.h"
#include "Benchmark.h"

int main() {
    PixelCanvas canvas;
    measureExecutionTime("Initialize canvas", &PixelCanvas::initCanvas, canvas, 1140, 912, (std::string) "Diamond", true);

    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Reset pattern", &PixelCanvas::resetPattern, canvas, (uint32_t) 0xFF000000, true);
    measureExecutionTime("Reset pattern (no parallel)", &PixelCanvas::resetPattern, canvas, (uint32_t) 0xFF000000, false);
    measureExecutionTime("Reset background", &PixelCanvas::resetBackground, canvas, (uint32_t) 0xFF0000, true);
    measureExecutionTime("Reset background (no parallel)", &PixelCanvas::resetBackground, canvas, (uint32_t) 0xFF0000, false);
    measureExecutionTime("Update pattern to real", &PixelCanvas::updatePattern2Real, canvas, true);
    measureExecutionTime("Update pattern to real (no parallel)", &PixelCanvas::updatePattern2Real, canvas, false);
    measureExecutionTime("Update real to pattern", &PixelCanvas::updateReal2Pattern, canvas, true);
    measureExecutionTime("Update real to pattern (no parallel)", &PixelCanvas::updateReal2Pattern, canvas, false);

    // Allocate valid pixel data instead of passing nullptr
    std::vector<uint32_t> pixelData(1140 * 912, 0xFFFFFF); // White pixels
    std::cout << "Pixel data created with size: " << pixelData.size() << std::endl;

    // measureExecutionTime("Copy pixel to pattern", &PixelCanvas::copyPixel2Pattern, canvas, pixelData.data(), pixelData.size(), true);
    // measureExecutionTime("Copy pixel to pattern (no parallel)", &PixelCanvas::copyPixel2Pattern, canvas, pixelData.data(), pixelData.size(), false);

    return 0;
}
