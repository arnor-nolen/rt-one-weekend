#include <cstddef>
#include <cstdlib>
#include <filesystem>

#include <rtweekend.hpp>

#include <camera.hpp>
#include <hittable_list.hpp>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int {

    // Image.
    constexpr auto aspectRatio = 16.0 / 9.0;
    constexpr size_t imageWidth = 400u;

    // World.
    auto world = HittableList{};

    world.add(std::make_shared<Sphere>(Point3{0, 0, -1}, 0.5));
    world.add(std::make_shared<Sphere>(Point3{0, -100.5, -1}, 100));

    auto camera = Camera{"output/image.png", aspectRatio, imageWidth};

    camera.render(world);

    return EXIT_SUCCESS;
}
