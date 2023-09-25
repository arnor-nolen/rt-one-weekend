#include <color.hpp>

namespace {

constexpr auto s_maxColorValue = 255.999;

} // namespace

void write_color(FILE *descriptor, Color color) {
    fmt::print(descriptor, "{} {} {}\n",
               static_cast<int>(s_maxColorValue * color[0]),
               static_cast<int>(s_maxColorValue * color[1]),
               static_cast<int>(s_maxColorValue * color[2]));
}
