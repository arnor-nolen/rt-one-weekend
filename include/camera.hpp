#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <filesystem>

#include <fmt/format.h>
#include <impl/cimg.hpp>

#include <rtweekend.hpp>

#include <color.hpp>
#include <hittable.hpp>
#include <sphere.hpp>

class Camera {
  public:
    explicit Camera(std::string_view outputPath, double aspectRatio = 1.0,
                    size_t imageWidth = 100, size_t samplesPerPixel = 10);

    void render(const CHittable auto &world) {

        for (size_t j = 0; j < m_imageHeight; ++j) {
            fmt::print("Scanlines remaining: {}.\n", m_imageHeight - j);

            for (size_t i = 0; i < m_imageWidth; ++i) {
                auto pixelColor = Color{0, 0, 0};

                for (size_t sample = 0; sample < m_samplesPerPixel; ++sample) {
                    auto ray = getRay(i, j);
                    pixelColor +=
                        convertColor(rayColor(ray, world), m_samplesPerPixel);
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
        m_image.save_png(m_outputPath.string().c_str());

        fmt::print("\nImage saved to \"{}\".\n", m_outputPath.string());
    }

  private:
    void initialize();

    auto rayColor(const Ray &ray, const CHittable auto &world) const -> Color {

        auto record = world.hit(ray, Interval{0, s_infinity});
        if (record) {
            return 0.5 * (record->normal + Color{1, 1, 1});
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

        auto rayOrigin = m_cameraCenter;
        auto rayDirection = pixelSample - rayOrigin;

        return Ray{rayOrigin, rayDirection};
    }

    [[nodiscard]]
    auto pixelSampleSquare() const -> Vec3 {
        auto pointX = -0.5 + randomDouble();
        auto pointY = -0.5 + randomDouble();

        return (pointX * m_pixelDeltaU) + (pointY * m_pixelDeltaV);
    }

    double m_aspectRatio;
    size_t m_imageWidth;

    size_t m_imageHeight{};
    Point3 m_cameraCenter{};
    Point3 m_pixel00Loc{};

    Vec3 m_pixelDeltaU{};
    Vec3 m_pixelDeltaV{};

    size_t m_samplesPerPixel{};

    cimg_library::CImg<uint8_t> m_image{};

    std::filesystem::path m_outputPath{};
};

#endif
