#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include <utility>

class Interval
{
  public:
    static const Interval s_empty;
    static const Interval s_universe;

    [[nodiscard]]
    static auto empty() noexcept -> Interval;

    [[nodiscard]]
    static auto universe() noexcept -> Interval;

    explicit Interval() noexcept;
    explicit Interval(double min, double max) noexcept;
    explicit Interval(std::pair<double, double> pair) noexcept;
    explicit Interval(const Interval &interval1,
                      const Interval &interval2) noexcept;

    [[nodiscard]]
    auto size() const noexcept -> double;

    [[nodiscard]]
    auto contains(double value) const noexcept -> bool;

    [[nodiscard]]
    auto surrounds(double value) const noexcept -> bool;

    [[nodiscard]]
    auto clamp(double value) const noexcept -> double;

    [[nodiscard]]
    auto expand(double delta) const noexcept -> Interval;

    [[nodiscard]]
    auto getMin() const noexcept -> double;

    [[nodiscard]]
    auto getMax() const noexcept -> double;

  private:
    double m_min;
    double m_max;
};

#endif
