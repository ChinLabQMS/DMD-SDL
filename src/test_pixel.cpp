#include <PixelCanvas.h>
#include <chrono>

// Template function to measure execution time of a non-static class member function
template <typename ClassType, typename ReturnType, typename... Args>
void measureExecutionTime(const std::string& label, ReturnType (ClassType::*func)(Args...), ClassType& obj, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    // Call the member function
    (obj.*func)(std::forward<Args>(args)...);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " execution time: " << elapsed.count() << " seconds" << std::endl;
}

int main() {
    PixelCanvas canvas;
    canvas.initCanvas(1140, 912);

    std::cout << "Canvas created with width: " << canvas.NumCols
              << ", height: " << canvas.NumRows 
              << ", real_width: " << canvas.RealNumCols 
              << ", real_height: " << canvas.RealNumRows << std::endl;

    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Reset canvas", &PixelCanvas::resetCanvas, canvas, (uint32_t) 0xFF000000, (uint32_t) 0xFF0000, true);
    measureExecutionTime("Reset canvas (no parallel)", &PixelCanvas::resetCanvas, canvas, (uint32_t) 0xFF000000, (uint32_t) 0xFF0000, false);
    measureExecutionTime("Update pattern to real", &PixelCanvas::updatePattern2Real, canvas, true);
    measureExecutionTime("Update pattern to real (no parallel)", &PixelCanvas::updatePattern2Real, canvas, false);
    measureExecutionTime("Update real to pattern", &PixelCanvas::updateReal2Pattern, canvas, true);
    measureExecutionTime("Update real to pattern (no parallel)", &PixelCanvas::updateReal2Pattern, canvas, false);

    // Allocate valid pixel data instead of passing nullptr
    std::vector<uint32_t> pixelData(1140 * 912, 0xFFFFFF); // White pixels
    std::cout << "Pixel data created with size: " << pixelData.size() << std::endl;

    measureExecutionTime("Copy pixel to pattern", &PixelCanvas::copyPixel2Pattern, canvas, pixelData.data(), pixelData.size(), true);
    measureExecutionTime("Copy pixel to pattern (no parallel)", &PixelCanvas::copyPixel2Pattern, canvas, pixelData.data(), pixelData.size(), false);

    return 0;
}
