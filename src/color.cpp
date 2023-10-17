#include <color.hpp>

#include <interval.hpp>

namespace {

constexpr auto s_maxColorValue = 255.999;

} // namespace

auto convert_color(const Color &color, size_t samplesPerPixel) noexcept
    -> Color {
    static const auto intensity = Interval{0.0, 0.999};

    auto scale = 1.0 / static_cast<double>(samplesPerPixel);

    return Color{s_maxColorValue * intensity.clamp(color[0] * scale),
                 s_maxColorValue * intensity.clamp(color[1] * scale),
                 s_maxColorValue * intensity.clamp(color[2] * scale)};
}
