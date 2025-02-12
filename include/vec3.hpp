#ifndef VEC3_HPP
#define VEC3_HPP

#include <array>
#include <cmath>
#include <cstddef>
#include <print>

#include <random.hpp>

class Vec3 {

  public:
    explicit Vec3() noexcept;
    explicit Vec3(double value0, double value1, double value2) noexcept;

    [[nodiscard]]
    auto getX() const noexcept -> double;

    [[nodiscard]]
    auto getY() const noexcept -> double;

    [[nodiscard]]
    auto getZ() const noexcept -> double;

    auto operator-() const noexcept -> Vec3;

    auto operator[](size_t index) const noexcept -> double;
    auto operator[](size_t index) noexcept -> double &;

    auto operator+=(const Vec3 &other) noexcept -> Vec3 &;
    auto operator*=(double multiplier) noexcept -> Vec3 &;
    auto operator/=(double divisor) -> Vec3 &;

    [[nodiscard]]
    auto length() const -> double;

    [[nodiscard]]
    auto lengthSquared() const noexcept -> double;

    [[nodiscard]]
    static auto random() noexcept -> Vec3;

    [[nodiscard]]
    static auto random(double min, double max) noexcept -> Vec3;

    [[nodiscard]]
    auto nearZero() const -> bool;

  private:
    std::array<double, 3> m_vec;
};

using Point3 = Vec3;

// Vec3 utility functions.

template <>
class std::formatter<Vec3> {
  public:
    static constexpr auto parse(std::format_parse_context &ctx);

    template <typename Context>
    static constexpr auto format(const Vec3 &vec3, Context &ctx) {
        return std::format_to(ctx.out(), "{} {} {}", vec3[0], vec3[1], vec3[2]);
    }
};

inline auto operator+(const Vec3 &lhs, const Vec3 &rhs) noexcept -> Vec3 {
    return Vec3{lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]};
}

inline auto operator-(const Vec3 &lhs, const Vec3 &rhs) noexcept -> Vec3 {
    return Vec3{lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]};
}

inline auto operator*(const Vec3 &lhs, const Vec3 &rhs) noexcept -> Vec3 {
    return Vec3{lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]};
}

inline auto operator*(double multiplier, const Vec3 &vec) noexcept -> Vec3 {
    return Vec3{multiplier * vec[0], multiplier * vec[1], multiplier * vec[2]};
}

inline auto operator*(const Vec3 &vec, double multiplier) noexcept -> Vec3 {
    return multiplier * vec;
}

inline auto operator/(const Vec3 &vec, double multiplier) -> Vec3 {
    return (1 / multiplier) * vec;
}

inline auto dot(const Vec3 &lhs, const Vec3 &rhs) noexcept -> double {
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

inline auto cross(const Vec3 &lhs, const Vec3 &rhs) noexcept -> Vec3 {
    return Vec3{lhs[1] * rhs[2] - lhs[2] * rhs[1],
                lhs[2] * rhs[0] - lhs[0] * rhs[2],
                lhs[0] * rhs[1] - lhs[1] * rhs[0]};
}

inline auto unitVector(const Vec3 &vec) -> Vec3 { return vec / vec.length(); }

inline auto randomInUnitSphere() noexcept -> Vec3 {
    while (true) {
        auto point = Vec3::random(-1, 1);
        if (point.lengthSquared() < 1) {
            return point;
        }
    }
}

inline auto randomUnitVector() -> Vec3 {
    return unitVector(randomInUnitSphere());
}

inline auto randomOnHemisphere(const Vec3 &normal) -> Vec3 {
    auto onUnitSphere = randomUnitVector();

    if (dot(onUnitSphere, normal) > 0.0) {
        return onUnitSphere;
    }

    return -onUnitSphere;
}

[[nodiscard]]
auto reflect(const Vec3 &vector, const Vec3 &normal) -> Vec3;

inline auto refract(const Vec3 &unitVector, const Vec3 &normal,
                    double etaiOverEtat) -> Vec3 {
    auto cosTheta = std::fmin(dot(-unitVector, normal), 1.0);
    Vec3 rOutPerp = etaiOverEtat * (unitVector + cosTheta * normal);
    Vec3 rOutParallel =
        -std::sqrt(std::fabs(1.0 - rOutPerp.lengthSquared())) * normal;
    return rOutPerp + rOutParallel;
}

inline auto randomInUnitDisk() -> Vec3 {
    while (true) {
        const auto point = Vec3{randomDouble(-1, 1), randomDouble(-1, 1), 0};
        if (point.lengthSquared() < 1) {
            return point;
        }
    };
};

#endif
