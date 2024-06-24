#include <interval.hpp>

#include <limits>

Interval::Interval() noexcept
    : m_min{+std::numeric_limits<double>::infinity()},
      m_max{-std::numeric_limits<double>::infinity()} {};

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

auto Interval::expand(double delta) const noexcept -> Interval {
    const auto padding = delta / 2;

    return Interval{m_min - padding, m_max + padding};
}

auto Interval::getMin() const noexcept -> double { return m_min; }
auto Interval::getMax() const noexcept -> double { return m_max; }

const static Interval s_empty{+std::numeric_limits<double>::infinity(),
                              -std::numeric_limits<double>::infinity()};
const static Interval s_universe{-std::numeric_limits<double>::infinity(),
                                 +std::numeric_limits<double>::infinity()};
