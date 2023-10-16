#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <hittable.hpp>
#include <vec3.hpp>

class Interval;

class Sphere {
  public:
    explicit Sphere(Point3 center, double radius) noexcept;

    [[nodiscard]]
    auto hit(const Ray &ray, Interval rayT) const noexcept
        -> std::optional<HitRecord>;

  private:
    Point3 m_center;
    double m_radius;
};

static_assert(CHittable<Sphere>);

#endif
