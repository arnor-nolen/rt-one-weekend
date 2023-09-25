#include <cstddef>
#include <cstdlib>
#include <fmt/format.h>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

    // Image.
    constexpr size_t image_width = 256u;
    constexpr size_t image_height = 256u;

    constexpr auto s_maxColorValue = 255.999;

    // Renderer.
    fmt::print("P3\n {} {} \n255\n", image_width, image_height);

    for (size_t j = 0; j < image_height; ++j) {
        fmt::print(stderr, "Scanlines remaining: {}.\n", image_height - j);
        for (size_t i = 0; i < image_width; ++i) {
            auto color_r = static_cast<double>(i) / (image_width - 1);
            auto color_g = static_cast<double>(j) / (image_width - 1);
            auto color_b = 0;

            int image_r = static_cast<int>(s_maxColorValue * color_r);
            int image_g = static_cast<int>(s_maxColorValue * color_g);
            int image_b = static_cast<int>(s_maxColorValue * color_b);

            fmt::print("{} {} {}\n", image_r, image_g, image_b);
        }
    }

    return EXIT_SUCCESS;
}
