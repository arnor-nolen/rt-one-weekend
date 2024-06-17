#include <cstddef>
#include <cstdlib>

#include <rtweekend.hpp>

#include <camera.hpp>
#include <color.hpp>
#include <hittable_list.hpp>
#include <material.hpp>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {
    try {

        static constexpr auto s_aspectRatio = 16.0 / 9.0;
        static constexpr size_t s_imageWidth = 1200u;
        static constexpr size_t s_samplesPerPixel = 500u;
        static constexpr size_t s_maxDepth = 50u;
        static constexpr double s_vFov = 20.0;
        static constexpr double s_defocusAngle = 0.6;
        static constexpr double s_focusDist = 10.0;

        auto world = HittableList{};

        auto materialGround = MaterialVariant{std::in_place_type<Lambertian>,
                                              Color{0.5, 0.5, 0.5}};
        world.add<Sphere>(Point3{0.0, -1000.0, 0.0}, 1000.0, materialGround);

        for (int coordA = -11; coordA < 11; ++coordA) {
            for (int coordB = -11; coordB < 11; ++coordB) {
                const auto chooseMat = randomDouble();
                auto center = Point3{coordA + 0.9 * randomDouble(), 0.2,
                                     coordB + 0.9 * randomDouble()};

                if ((center - Point3{4.0, 0.2, 0.0}).length() > 0.9) {

                    if (chooseMat < 0.8) {
                        const auto albedo = Color::random() * Color::random();
                        const auto sphereMaterial = MaterialVariant{
                            std::in_place_type<Lambertian>, albedo};
                        const auto center2 =
                            center + Vec3{0, randomDouble(0, 0.5), 0};
                        world.add<Sphere>(center, center2, 0.2, sphereMaterial);

                    } else if (chooseMat < 0.95) {
                        const auto albedo = Color::random(0.5, 1.0);
                        const auto fuzz = randomDouble(0.0, 0.5);
                        const auto sphereMaterial = MaterialVariant{
                            std::in_place_type<Metal>, albedo, fuzz};
                        world.add<Sphere>(center, 0.2, sphereMaterial);

                    } else {
                        const auto sphereMaterial = MaterialVariant{
                            std::in_place_type<Dielectric>, 1.5};
                        world.add<Sphere>(center, 0.2, sphereMaterial);
                    }
                }
            }
        }

        auto material1 = MaterialVariant{std::in_place_type<Dielectric>, 1.5};
        world.add<Sphere>(Point3{0, 1, 0}, 1.0, material1);

        auto material2 = MaterialVariant{std::in_place_type<Lambertian>,
                                         Color{0.4, 0.2, 0.1}};
        world.add<Sphere>(Point3{-4.0, 1.0, 0.0}, 1.0, material2);

        auto material3 = MaterialVariant{std::in_place_type<Metal>,
                                         Color{0.7, 0.6, 0.5}, 0.0};
        world.add<Sphere>(Point3{4, 1, 0}, 1.0, material3);

        auto cameraProps = CameraProps{.outputPath = "output/image.png",
                                       .aspectRatio = s_aspectRatio,
                                       .imageWidth = s_imageWidth,
                                       .samplesPerPixel = s_samplesPerPixel,
                                       .maxDepth = s_maxDepth,
                                       .vFov = s_vFov,
                                       .lookFrom = Point3{13, 2, 3},
                                       .lookAt = Point3{0, 0, 0},
                                       .vUp = Vec3{0, 1, 0},
                                       .defocusAngle = s_defocusAngle,
                                       .focusDist = s_focusDist};

        auto camera = Camera{cameraProps};

        camera.render(world);

    } catch (const std::exception &e) {
        fmt::print("Exception: {}.", e.what());
    }

    return EXIT_SUCCESS;
}
