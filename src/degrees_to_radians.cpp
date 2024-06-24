#include <degrees_to_radians.hpp>

#include <numbers>

auto degreesToRadians(double degrees) noexcept -> double {
    return degrees * std::numbers::pi / 180.0;
}
