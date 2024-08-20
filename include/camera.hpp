#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <filesystem>
#include <limits>
#include <ranges>
#include <thread>

#include <fmt/format.h>
#include <impl/cimg.hpp>

#include <color.hpp>
#include <hittable.hpp>
#include <map_reduce.hpp>
#include <material.hpp>
#include <sphere.hpp>

struct CameraProps {
    std::filesystem::path outputPath;
    double aspectRatio = 1.0;
    size_t imageWidth = 100;
    size_t samplesPerPixel = 10;
    size_t maxDepth = 10;
    double vFov = 90;
    Point3 lookFrom = Point3{0, 0, -1};
    Point3 lookAt = Point3{0, 0, 0};
    Vec3 vUp = Vec3{0, 1, 0};
    double defocusAngle = 0.0;
    double focusDist = 10.0;
};

class Camera {
  public:
    explicit Camera(CameraProps cameraProps);

    void render(const concepts::Hittable auto &world);

  private:
    void initialize();

    auto rayColor(const Ray &ray, size_t depth,
                  const concepts::Hittable auto &world) const -> Color;

    [[nodiscard]]
    auto getRay(size_t iCoord, size_t jCoord) const -> Ray;

    [[nodiscard]]
    auto pixelSampleSquare() const -> Vec3;

    [[nodiscard]]
    auto defocusDiskSample() const -> Point3;

    CameraProps m_cameraProps{};

    size_t m_imageHeight{};
    Point3 m_cameraCenter{};
    Point3 m_pixel00Loc{};

    Vec3 m_pixelDeltaU{};
    Vec3 m_pixelDeltaV{};

    Vec3 m_cameraU{};
    Vec3 m_cameraV{};
    Vec3 m_cameraW{};

    Vec3 m_defocusDiskU{};
    Vec3 m_defocusDiskV{};

    cimg_library::CImg<uint8_t> m_image{};
};

void Camera::render(const concepts::Hittable auto &world) {
    const auto numOfPixels = m_image.width() * m_image.height();
    const auto range = std::ranges::views::iota(0, numOfPixels);

    parallelize(
        range,
        [&, this](const auto &pixelId) {
            auto pixelColor = Color{0, 0, 0};

            auto idxW = pixelId / m_image.height();
            auto idxH = pixelId % m_image.height();

            for (size_t sample = 0; sample < m_cameraProps.samplesPerPixel;
                 ++sample) {
                auto ray = getRay(idxW, idxH);
                pixelColor +=
                    convertColor(rayColor(ray, m_cameraProps.maxDepth, world),
                                 m_cameraProps.samplesPerPixel);
            }

            auto colorArray =
                std::array<uint8_t, 3>{static_cast<uint8_t>(pixelColor.getX()),
                                       static_cast<uint8_t>(pixelColor.getY()),
                                       static_cast<uint8_t>(pixelColor.getZ())};

            m_image.draw_point(static_cast<int>(idxW), static_cast<int>(idxH),
                               colorArray.data());
            return true;
        },
        std::thread::hardware_concurrency());

    // Windows doesn't play nicely with save_png and
    // std::filesystem, so we have to convert twice.
    m_image.save_png(m_cameraProps.outputPath.string().c_str());

    fmt::print("\nImage saved to \"{}\".\n", m_cameraProps.outputPath.string());
}

auto Camera::rayColor(const Ray &ray, size_t depth,
                      const concepts::Hittable auto &world) const -> Color {
    auto outputColor = Color{1.0, 1.0, 1.0};
    auto currentRay = ray;

    for (size_t i = 0u; i < depth; ++i) {
        const auto record = world.hit(
            currentRay,
            Interval{0.001, +std::numeric_limits<double>::infinity()});

        // Color background.
        if (!record) {
            Vec3 unitDirection = unitVector(currentRay.direction());
            const auto currentValue = 0.5 * (unitDirection.getY() + 1.0);

            const auto skyColor = (1.0 - currentValue) * Color{1.0, 1.0, 1.0} +
                                  currentValue * Color{0.5, 0.7, 1.0};

            return outputColor * skyColor;
        }

        const auto scatterInfo = std::visit(
            [&](const concepts::Material auto &elem) {
                return elem.scatter(currentRay, *record);
            },
            record->material);

        if (!scatterInfo) {
            return Color{0, 0, 0};
        }

        outputColor = outputColor * scatterInfo->attenuation;
        currentRay = scatterInfo->scattered;
    }

    // Recursion limit exceeded.
    return Color{0, 0, 0};
}

#endif
