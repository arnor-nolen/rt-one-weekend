#include <cstddef>
#include <cstdlib>
#include <filesystem>

#include <rtweekend.hpp>

#include <camera.hpp>
#include <color.hpp>
#include <hittable_list.hpp>
#include <material.hpp>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

    static constexpr auto s_aspectRatio = 16.0 / 9.0;
    static constexpr size_t s_imageWidth = 400u;
    static constexpr size_t s_samplesPerPixel = 100u;
    static constexpr size_t s_maxDepth = 50u;

    auto world = HittableList{};

    auto materialGround =
        MaterialVariant{std::in_place_type<Lambertian>, Color{0.8, 0.8, 0.0}};
    auto materialCenter =
        MaterialVariant{std::in_place_type<Lambertian>, Color{0.1, 0.2, 0.5}};
    auto materialLeft = MaterialVariant{std::in_place_type<Dielectric>, 1.5};
    auto materialRight =
        MaterialVariant{std::in_place_type<Metal>, Color{0.8, 0.6, 0.2}, 0.0};

    world.add(std::make_shared<Sphere>(Point3{0.0, -100.5, -1.0}, 100.0,
                                       materialGround));
    world.add(
        std::make_shared<Sphere>(Point3{0.0, 0.0, -1.0}, 0.5, materialCenter));
    world.add(
        std::make_shared<Sphere>(Point3{-1.0, 0.0, -1.0}, 0.5, materialLeft));
    world.add(
        std::make_shared<Sphere>(Point3{-1.0, 0.0, -1.0}, -0.4, materialLeft));
    world.add(
        std::make_shared<Sphere>(Point3{1.0, 0.0, -1.0}, 0.5, materialRight));

    auto cameraProps = CameraProps{.outputPath = "output/image.png",
                                   .aspectRatio = s_aspectRatio,
                                   .imageWidth = s_imageWidth,
                                   .samplesPerPixel = s_samplesPerPixel,
                                   .maxDepth = s_maxDepth};

    auto camera = Camera{cameraProps};

    camera.render(world);

    return EXIT_SUCCESS;
}
