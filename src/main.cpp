#include <cstddef>
#include <cstdlib>
#include <filesystem>

#include <rtweekend.hpp>

#include <camera.hpp>
#include <hittable_list.hpp>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

    constexpr static auto s_aspectRatio = 16.0 / 9.0;
    constexpr static size_t s_imageWidth = 400u;
    constexpr static size_t s_samplesPerPixel = 100u;
    constexpr static size_t s_maxDepth = 50u;

    auto world = HittableList{};

    world.add(std::make_shared<Sphere>(Point3{0, 0, -1}, 0.5));
    world.add(std::make_shared<Sphere>(Point3{0, -100.5, -1}, 100));

    auto cameraProps = CameraProps{.outputPath = "output/image.png",
                                   .aspectRatio = s_aspectRatio,
                                   .imageWidth = s_imageWidth,
                                   .samplesPerPixel = s_samplesPerPixel,
                                   .maxDepth = s_maxDepth};

    auto camera = Camera{cameraProps};

    camera.render(world);

    return EXIT_SUCCESS;
}
