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
                    size_t imageWidth = 100);

    void render(const CHittable auto &world) {

        for (size_t j = 0; j < m_imageHeight; ++j) {
            fmt::print("Scanlines remaining: {}.\n", m_imageHeight - j);
            for (size_t i = 0; i < m_imageWidth; ++i) {
                auto pixelCenter = m_pixel00Loc +
                                   (static_cast<double>(i) * m_pixelDeltaU) +
                                   (static_cast<double>(j) * m_pixelDeltaV);

                auto rayDirection = pixelCenter - m_cameraCenter;
                auto ray = Ray{m_cameraCenter, rayDirection};

                auto pixelColor = convert_color(ray_color(ray, world));

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

    auto ray_color(const Ray &ray, const CHittable auto &world) const -> Color {

        auto record = world.hit(ray, Interval{0, s_infinity});
        if (record) {
            return 0.5 * (record->normal + Color{1, 1, 1});
        }

        // Color background.
        Vec3 unitDirection = unit_vector(ray.direction());
        const auto currentValue = 0.5 * (unitDirection.getY() + 1.0);

        return (1.0 - currentValue) * Color{1.0, 1.0, 1.0} +
               currentValue * Color{0.5, 0.7, 1.0};
    }

    double m_aspectRatio;
    size_t m_imageWidth;

    size_t m_imageHeight{};
    Point3 m_cameraCenter{};
    Point3 m_pixel00Loc{};

    Vec3 m_pixelDeltaU{};
    Vec3 m_pixelDeltaV{};

    cimg_library::CImg<uint8_t> m_image{};

    std::filesystem::path m_outputPath{};
};

#endif
