#ifndef RAY_HPP
#define RAY_HPP

#include <vec3.hpp>

class Ray {
  public:
    explicit Ray() noexcept = default;
    Ray(const Point3 &origin, const Vec3 &direction) noexcept;

    [[nodiscard]]
    auto origin() const noexcept -> Point3;

    [[nodiscard]]
    auto direction() const noexcept -> Vec3;

    [[nodiscard]]
    auto at(double time) const noexcept -> Point3;

  private:
    Point3 m_origin;
    Vec3 m_direction;
};

#endif
