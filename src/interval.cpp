#include <interval.hpp>

#include <rtweekend.hpp>

Interval::Interval() noexcept : m_min{+s_infinity}, m_max{-s_infinity} {};

Interval::Interval(double min, double max) noexcept : m_min{min}, m_max{max} {};

auto Interval::contains(double value) const noexcept -> bool {
    return m_min <= value && value <= m_max;
}

auto Interval::surrounds(double value) const noexcept -> bool {
    return m_min < value && value < m_max;
}

auto Interval::clamp(double value) const noexcept -> double {
    if (value < m_min) {
        return m_min;
    }

    if (value > m_max) {
        return m_max;
    }

    return value;
}

auto Interval::getMin() const noexcept -> double { return m_min; }
auto Interval::getMax() const noexcept -> double { return m_max; }

const static Interval s_empty{+s_infinity, -s_infinity};
const static Interval s_universe{-s_infinity, +s_infinity};
