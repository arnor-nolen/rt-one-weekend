#ifndef COLOR_HPP
#define COLOR_HPP

#include <fmt/format.h>

#include <vec3.hpp>

using Color = Vec3;

auto convert_color(const Color &color) noexcept -> Color;
void write_color(FILE *descriptor, Color color);

#endif
