#include <vec3.hpp>

#include <cassert>
#include <cmath>

Vec3::Vec3() noexcept : m_vec{0, 0, 0} {}

Vec3::Vec3(double value0, double value1, double value2) noexcept
    : m_vec{value0, value1, value2} {}

auto Vec3::getX() const noexcept -> double { return m_vec[0]; }

auto Vec3::getY() const noexcept -> double { return m_vec[1]; }

auto Vec3::getZ() const noexcept -> double { return m_vec[2]; }

auto Vec3::operator-() const noexcept -> Vec3 {
    return Vec3{-m_vec[0], -m_vec[1], -m_vec[2]};
}

auto Vec3::operator[](size_t index) const noexcept -> double {
    assert(index <= 2u);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    return m_vec[index];
}

auto Vec3::operator[](size_t index) noexcept -> double & {
    assert(index <= 2u);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    return m_vec[index];
}

auto Vec3::operator+=(const Vec3 &other) noexcept -> Vec3 & {
    m_vec[0] += other[0];
    m_vec[1] += other[1];
    m_vec[2] += other[2];

    return *this;
}

auto Vec3::operator*=(double multiplier) noexcept -> Vec3 & {
    m_vec[0] *= multiplier;
    m_vec[1] *= multiplier;
    m_vec[2] *= multiplier;

    return *this;
}

auto Vec3::operator/=(double divisor) -> Vec3 & { return *this *= 1 / divisor; }

auto Vec3::length() const -> double { return std::sqrt(lengthSquared()); }

auto Vec3::lengthSquared() const noexcept -> double {
    return m_vec[0] * m_vec[0] + m_vec[1] * m_vec[1] + m_vec[2] * m_vec[2];
}

constexpr auto fmt::formatter<Vec3>::parse(fmt::format_parse_context &ctx) {
    return ctx.begin();
}
