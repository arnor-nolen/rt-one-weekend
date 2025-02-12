#ifndef COLOR_HPP
#define COLOR_HPP

#include <vec3.hpp>

using Color = Vec3;

auto convertColor(const Color &color, size_t samplesPerPixel) noexcept -> Color;

inline auto linearToGamma(double linearComponent) -> double {
    return std::sqrt(linearComponent);
}

#endif
