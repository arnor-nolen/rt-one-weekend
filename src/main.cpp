#include <cstddef>
#include <cstdlib>

#include <fmt/format.h>

#include <color.hpp>
#include <vec3.hpp>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

    // Image.
    constexpr size_t imageWidth = 256u;
    constexpr size_t imageHeight = 256u;

    // Renderer.
    fmt::print("P3\n {} {} \n255\n", imageWidth, imageHeight);

    for (size_t j = 0; j < imageHeight; ++j) {
        fmt::print(stderr, "Scanlines remaining: {}.\n", imageHeight - j);
        for (size_t i = 0; i < imageWidth; ++i) {
            auto pixelColor =
                Color{static_cast<double>(i) / (imageWidth - 1),
                      static_cast<double>(j) / (imageHeight - 1), 0};

            write_color(stdout, pixelColor);
        }
    }

    return EXIT_SUCCESS;
}
