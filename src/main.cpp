#include <cstddef>
#include <cstdlib>

#include <fmt/format.h>

#include <color.hpp>
#include <ray.hpp>
#include <vec3.hpp>

auto ray_color(const Ray &ray) noexcept -> Color {
    Vec3 unitDirection = unit_vector(ray.direction());
    const auto currentValue = 0.5 * (unitDirection.y() + 1.0);

    return (1.0 - currentValue) * Color{1.0, 1.0, 1.0} +
           currentValue * Color{0.5, 0.7, 1.0};
}

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

    // Image.
    constexpr auto aspectRatio = 16.0 / 9.0;
    constexpr size_t imageWidth = 400u;

    auto imageHeight = static_cast<size_t>(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    constexpr auto focalLength = 1.0;
    constexpr auto viewportHeight = 2.0;
    const auto viewportWidth =
        viewportHeight *
        (static_cast<double>(imageWidth) / static_cast<double>(imageHeight));
    const auto cameraCenter = Point3{};

    auto viewportU = Vec3{viewportWidth, 0, 0};
    auto viewportV = Vec3{0, -viewportHeight, 0};

    auto pixelDeltaU = viewportU / imageWidth;
    auto pixelDeltaV = viewportV / static_cast<double>(imageHeight);

    auto viewportUpperLeft =
        cameraCenter - Vec3{0, 0, focalLength} - viewportU / 2 - viewportV / 2;
    auto pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    // Renderer.
    fmt::print("P3\n {} {} \n255\n", imageWidth, imageHeight);

    for (size_t j = 0; j < imageHeight; ++j) {
        fmt::print(stderr, "Scanlines remaining: {}.\n", imageHeight - j);
        for (size_t i = 0; i < imageWidth; ++i) {
            auto pixelCenter = pixel00Loc +
                               (static_cast<double>(i) * pixelDeltaU) +
                               (static_cast<double>(j) * pixelDeltaV);
            auto rayDirection = pixelCenter - cameraCenter;
            auto ray = Ray{cameraCenter, rayDirection};

            auto pixelColor = ray_color(ray);

            write_color(stdout, pixelColor);
        }
    }

    return EXIT_SUCCESS;
}
