#ifndef RT_WEEKEND_HPP
#define RT_WEEKEND_HPP

#include <limits>
#include <memory>
#include <numbers>
#include <random>

#include <interval.hpp>
#include <ray.hpp>
#include <vec3.hpp>

constexpr double s_infinity = std::numeric_limits<double>::infinity();
constexpr double s_pi = std::numbers::pi;

[[nodiscard]]
inline auto degrees_to_radians(double degrees) noexcept -> double {
    return degrees * s_pi / 180.0;
}

[[nodiscard]]
inline auto random_double() -> double {
    static auto distribution = std::uniform_real_distribution<double>{0.0, 1.0};
    // NOLINTNEXTLINE(cert-msc32-c, cert-msc51-cpp)
    static auto generator = std::mt19937{};

    return distribution(generator);
};

[[nodiscard]]
inline auto random_double(double min, double max) -> double {
    return min + (max - min) * random_double();
};

#endif
