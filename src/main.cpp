#include <cstddef>
#include <cstdlib>
#include <filesystem>

#include <fmt/format.h>
#include <impls/cimg.hpp>

#include <color.hpp>
#include <ray.hpp>
#include <vec3.hpp>

auto hit_sphere(const Point3 &center, double radius, const Ray &ray) -> bool {
    auto vec = ray.origin() - center;

    auto quadA = dot(ray.direction(), ray.direction());
    auto quadB = 2.0 * dot(vec, ray.direction());
    auto quadC = dot(vec, vec) - radius * radius;

    auto discriminant = quadB * quadB - 4 * quadA * quadC;

    return discriminant >= 0;
}

auto ray_color(const Ray &ray) noexcept -> Color {
    if (hit_sphere(Point3{0, 0, -1}, 0.5, ray)) {
        return Color{1, 0, 0};
    }

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
    auto image = cimg_library::CImg<uint8_t>(
        static_cast<unsigned int>(imageWidth),
        static_cast<unsigned int>(imageHeight), 1u, 3u, 0u);

    for (size_t j = 0; j < imageHeight; ++j) {
        fmt::print("Scanlines remaining: {}.\n", imageHeight - j);
        for (size_t i = 0; i < imageWidth; ++i) {
            auto pixelCenter = pixel00Loc +
                               (static_cast<double>(i) * pixelDeltaU) +
                               (static_cast<double>(j) * pixelDeltaV);

            auto rayDirection = pixelCenter - cameraCenter;
            auto ray = Ray{cameraCenter, rayDirection};

            auto pixelColor = convert_color(ray_color(ray));

            auto colorArray =
                std::array<uint8_t, 3>{static_cast<uint8_t>(pixelColor.x()),
                                       static_cast<uint8_t>(pixelColor.y()),
                                       static_cast<uint8_t>(pixelColor.z())};

            image.draw_point(static_cast<int>(i), static_cast<int>(j),
                             colorArray.data());
        }
    }

    // Windows doesn't play nicely with save_png and std::filesystem, so we
    // have to convert twice.
    const auto outputPath = std::filesystem::path("./output/image.png");
    image.save_png(outputPath.string().c_str());

    fmt::print("\nImage saved to \"{}\".\n", outputPath.string());

    return EXIT_SUCCESS;
}
