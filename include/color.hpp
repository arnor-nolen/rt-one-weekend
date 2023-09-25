#ifndef COLOR_HPP
#define COLOR_HPP

#include <fmt/format.h>

#include <vec3.hpp>

using Color = Vec3;

void write_color(FILE *descriptor, Color color);

#endif
