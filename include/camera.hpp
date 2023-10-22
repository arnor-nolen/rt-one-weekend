#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <filesystem>

#include <fmt/format.h>
#include <impl/cimg.hpp>

#include <rtweekend.hpp>

#include <color.hpp>
#include <hittable.hpp>
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

    void render(const CHittable auto &world) {

        for (size_t j = 0; j < m_imageHeight; ++j) {
            fmt::print("Scanlines remaining: {}.\n", m_imageHeight - j);

            for (size_t i = 0; i < m_cameraProps.imageWidth; ++i) {
                auto pixelColor = Color{0, 0, 0};

                for (size_t sample = 0; sample < m_cameraProps.samplesPerPixel;
                     ++sample) {
                    auto ray = getRay(i, j);
                    pixelColor += convertColor(
                        rayColor(ray, m_cameraProps.maxDepth, world),
                        m_cameraProps.samplesPerPixel);
                }

                auto colorArray = std::array<uint8_t, 3>{
                    static_cast<uint8_t>(pixelColor.getX()),
                    static_cast<uint8_t>(pixelColor.getY()),
                    static_cast<uint8_t>(pixelColor.getZ())};

                m_image.draw_point(static_cast<int>(i), static_cast<int>(j),
                                   colorArray.data());
            }
        }

        // Windows doesn't play nicely with save_png and
        // std::filesystem, so we have to convert twice.
        m_image.save_png(m_cameraProps.outputPath.string().c_str());

        fmt::print("\nImage saved to \"{}\".\n",
                   m_cameraProps.outputPath.string());
    }

  private:
    void initialize();

    auto rayColor(const Ray &ray, size_t depth,
                  const CHittable auto &world) const -> Color {

        if (depth == 0) {
            return Color{0, 0, 0};
        }

        if (const auto record = world.hit(ray, Interval{0.001, s_infinity})) {

            const auto scatterInfo = std::visit(
                [&](const CMaterial auto &elem) {
                    return elem.scatter(ray, *record);
                },
                record->material);

            if (scatterInfo) {
                return scatterInfo->attenuation *
                       rayColor(scatterInfo->scattered, depth - 1, world);
            }

            return Color{0, 0, 0};
        }

        // Color background.
        Vec3 unitDirection = unitVector(ray.direction());
        const auto currentValue = 0.5 * (unitDirection.getY() + 1.0);

        return (1.0 - currentValue) * Color{1.0, 1.0, 1.0} +
               currentValue * Color{0.5, 0.7, 1.0};
    }

    [[nodiscard]]
    auto getRay(size_t iCoord, size_t jCoord) const -> Ray {
        auto pixelCenter = m_pixel00Loc +
                           (static_cast<double>(iCoord) * m_pixelDeltaU) +
                           (static_cast<double>(jCoord) * m_pixelDeltaV);
        auto pixelSample = pixelCenter + pixelSampleSquare();

        auto rayOrigin = m_cameraProps.defocusAngle <= 0 ? m_cameraCenter
                                                         : defocusDiskSample();
        auto rayDirection = pixelSample - rayOrigin;

        return Ray{rayOrigin, rayDirection};
    }

    [[nodiscard]]
    auto pixelSampleSquare() const -> Vec3 {
        auto pointX = -0.5 + randomDouble();
        auto pointY = -0.5 + randomDouble();

        return (pointX * m_pixelDeltaU) + (pointY * m_pixelDeltaV);
    }

    [[nodiscard]]
    auto defocusDiskSample() const -> Point3 {
        const auto point = randomInUnitDisk();
        return m_cameraCenter + (point.getX() * m_defocusDiskU) +
               (point.getY() * m_defocusDiskV);
    }

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

#endif
