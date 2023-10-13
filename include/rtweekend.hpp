#ifndef RT_WEEKEND_HPP
#define RT_WEEKEND_HPP

#include <limits>
#include <memory>
#include <numbers>

constexpr double s_infinity = std::numeric_limits<double>::infinity();
constexpr double s_pi = std::numbers::pi;

[[nodiscard]]
inline auto degrees_to_radians(double degrees) -> double {
    return degrees * s_pi / 180.0;
}

// Common headers.
#include <ray.hpp>
#include <vec3.hpp>

#endif
