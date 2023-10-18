#ifndef COLOR_HPP
#define COLOR_HPP

#include <fmt/format.h>

#include <vec3.hpp>

using Color = Vec3;

auto convertColor(const Color &color, size_t samplesPerPixel) noexcept -> Color;

#endif
