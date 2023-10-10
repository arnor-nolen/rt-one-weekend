#include <color.hpp>

namespace {

constexpr auto s_maxColorValue = 255.999;

} // namespace

auto convert_color(const Color &color) noexcept -> Color {
    return Color{s_maxColorValue * color[0], s_maxColorValue * color[1],
                 s_maxColorValue * color[2]};
}

void write_color(FILE *descriptor, Color color) {
    auto converted = convert_color(color);

    fmt::print(descriptor, "{} {} {}\n",
               static_cast<unsigned int>(converted[0]),
               static_cast<unsigned int>(converted[1]),
               static_cast<unsigned int>(converted[2]));
}
