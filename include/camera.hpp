#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <color.hpp>
#include <hittable.hpp>
#include <map_reduce.hpp>
#include <material.hpp>
#include <sphere.hpp>

#include <impl/cimg.hpp>

#include <atomic>
#include <cstdio>
#include <filesystem>
#include <limits>
#include <print>

#if !DEBUG
#include <ranges>
#include <thread>
#endif

struct CameraProps
{
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

class Camera
{
  public:
    explicit Camera(CameraProps cameraProps);

    void render(const concepts::Hittable auto &world);

  private:
    void initialize();

    auto rayColor(const Ray &ray,
                  size_t depth,
                  const concepts::Hittable auto &world) const -> Color;

    [[nodiscard]]
    auto getRay(size_t iCoord, size_t jCoord) const -> Ray;

    [[nodiscard]]
    auto pixelSampleSquare() const -> Vec3;

    [[nodiscard]]
    auto defocusDiskSample() const -> Point3;

    [[nodiscard]]
    auto
    calculatePixelColor(int idxW,
                        int idxH,
                        const concepts::Hittable auto &world) const noexcept
        -> Color;

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

auto Camera::calculatePixelColor(
    int idxW,
    int idxH,
    const concepts::Hittable auto &world) const noexcept -> Color
{

    auto pixelColor = Color{0, 0, 0};

    for (size_t sample = 0; sample < m_cameraProps.samplesPerPixel; ++sample)
    {
        const auto ray = getRay(idxW, idxH);
        const auto singleRayColor
            = rayColor(ray, m_cameraProps.maxDepth, world);
        pixelColor
            += convertColor(singleRayColor, m_cameraProps.samplesPerPixel);
    }

    return pixelColor;
}

void Camera::render(const concepts::Hittable auto &world)
{
    const auto numOfPixels = m_image.width() * m_image.height();

    auto progress = std::atomic<size_t>{0};

    const auto lambda = [&, this](const auto &pixelId)
    {
        const auto idxW = pixelId / m_image.height();
        const auto idxH = pixelId % m_image.height();

        const auto color = calculatePixelColor(idxW, idxH, world);

        const auto colorArray = std::array{static_cast<uint8_t>(color.getX()),
                                           static_cast<uint8_t>(color.getY()),
                                           static_cast<uint8_t>(color.getZ())};

        m_image.draw_point(idxW, idxH, colorArray.data());

        std::print("\rPixels processed: {}/{}.", ++progress, numOfPixels);

        [[maybe_unused]]
        const auto flushResult
            = std::fflush(stdout);
    };

    const auto range = std::ranges::views::iota(0, numOfPixels);

#if DEBUG
    std::ranges::for_each(range, lambda);
#else
    parallelizeForEach(range, lambda, std::thread::hardware_concurrency());
#endif

    // Windows doesn't play nicely with save_png and
    // std::filesystem, so we have to convert twice.
    m_image.save_png(m_cameraProps.outputPath.string().c_str());

    std::println("\nImage saved to \"{}\".", m_cameraProps.outputPath.string());
}

auto Camera::rayColor(const Ray &ray,
                      size_t depth,
                      const concepts::Hittable auto &world) const -> Color
{

    auto outputColor = Color{1.0, 1.0, 1.0};
    auto currentRay = ray;

    for (size_t i = 0u; i < depth; ++i)
    {
        const auto record = world.hit(
            currentRay,
            Interval{0.001, +std::numeric_limits<double>::infinity()});

        // Color background.
        if (!record)
        {
            Vec3 unitDirection = unitVector(currentRay.direction());
            const auto currentValue = 0.5 * (unitDirection.getY() + 1.0);

            const auto skyColor = (1.0 - currentValue) * Color{1.0, 1.0, 1.0}
                                + currentValue * Color{0.5, 0.7, 1.0};

            return outputColor * skyColor;
        }

        const auto scatterInfo
            = std::visit([&](const concepts::Material auto &elem)
                         { return elem.scatter(currentRay, *record); },
                         record->material);

        if (!scatterInfo)
        {
            return Color{0, 0, 0};
        }

        outputColor = outputColor * scatterInfo->attenuation;
        currentRay = scatterInfo->scattered;
    }

    // Recursion limit exceeded.
    return Color{0, 0, 0};
}

#endif
