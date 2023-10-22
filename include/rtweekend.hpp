#ifndef RTWEEKEND_HPP
#define RTWEEKEND_HPP

#include <limits>
#include <memory>
#include <numbers>
#include <random>

#include <interval.hpp>

constexpr double s_infinity = std::numeric_limits<double>::infinity();
constexpr double s_pi = std::numbers::pi;

[[nodiscard]]
inline auto degreesToRadians(double degrees) noexcept -> double {
    return degrees * s_pi / 180.0;
}

[[nodiscard]]
inline auto randomDouble() -> double {
    static auto s_distribution =
        std::uniform_real_distribution<double>{0.0, 1.0};
    // NOLINTNEXTLINE(cert-msc32-c, cert-msc51-cpp)
    static auto s_generator = std::mt19937{};

    return s_distribution(s_generator);
};

[[nodiscard]]
inline auto randomDouble(double min, double max) -> double {
    return min + (max - min) * randomDouble();
};

#endif
