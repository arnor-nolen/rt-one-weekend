#ifndef INTERVAL_HPP
#define INTERVAL_HPP

class Interval {
  public:
    static const Interval empty;
    static const Interval universe;

    explicit Interval() noexcept;
    explicit Interval(double min, double max) noexcept;

    [[nodiscard]]
    auto contains(double value) const noexcept -> bool;

    [[nodiscard]]
    auto surrounds(double value) const noexcept -> bool;

    [[nodiscard]]
    auto getMin() const noexcept -> double;

    [[nodiscard]]
    auto getMax() const noexcept -> double;

  private:
    double m_min;
    double m_max;
};

#endif
