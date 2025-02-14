#include <interval.hpp>

#include <algorithm>
#include <limits>

Interval::Interval() noexcept
    : m_min{+std::numeric_limits<double>::infinity()}
    , m_max{-std::numeric_limits<double>::infinity()} {};

Interval::Interval(double min, double max) noexcept
    : m_min{min}
    , m_max{max} {};

Interval::Interval(std::pair<double, double> pair) noexcept
    : m_min{pair.first}
    , m_max{pair.second}
{}

Interval::Interval(const Interval &interval1,
                   const Interval &interval2) noexcept
    : m_min{std::min(interval1.getMin(), interval2.getMin())}
    , m_max{std::max(interval1.getMax(), interval2.getMax())}
{}

auto Interval::size() const noexcept -> double { return m_max - m_min; }

auto Interval::contains(double value) const noexcept -> bool
{
    return m_min <= value && value <= m_max;
}

auto Interval::surrounds(double value) const noexcept -> bool
{
    return m_min < value && value < m_max;
}

auto Interval::clamp(double value) const noexcept -> double
{
    return std::clamp(value, m_min, m_max);
}

auto Interval::expand(double delta) const noexcept -> Interval
{
    const auto padding = delta / 2;

    return Interval{m_min - padding, m_max + padding};
}

auto Interval::getMin() const noexcept -> double { return m_min; }
auto Interval::getMax() const noexcept -> double { return m_max; }

const Interval Interval::s_empty{+std::numeric_limits<double>::infinity(),
                                 -std::numeric_limits<double>::infinity()};
const Interval Interval::s_universe{-std::numeric_limits<double>::infinity(),
                                    +std::numeric_limits<double>::infinity()};

auto Interval::empty() noexcept -> Interval { return s_empty; }
auto Interval::universe() noexcept -> Interval { return s_universe; }
