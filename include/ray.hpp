#ifndef RAY_HPP
#define RAY_HPP

#include <vec3.hpp>

class Ray {
  public:
    explicit Ray() noexcept = default;
    explicit Ray(const Point3 &origin, const Vec3 &direction) noexcept;
    explicit Ray(const Point3 &origin, const Vec3 &direction,
                 double time) noexcept;

    [[nodiscard]]
    auto origin() const noexcept -> Point3;

    [[nodiscard]]
    auto direction() const noexcept -> Vec3;

    [[nodiscard]]
    auto at(double time) const noexcept -> Point3;

    [[nodiscard]]
    auto time() const noexcept -> double;

  private:
    Point3 m_origin;
    Vec3 m_direction;
    double m_time{0.0};
};

#endif
