#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <hittable.hpp>
#include <material.hpp>
#include <variant>
#include <vec3.hpp>

class Interval;

class Sphere {
  public:
    explicit Sphere(const Point3 &center, double radius,
                    MaterialVariant material) noexcept;

    [[nodiscard]]
    auto hit(const Ray &ray, Interval rayT) const noexcept
        -> std::optional<HitRecord>;

  private:
    Point3 m_center;
    double m_radius;
    MaterialVariant m_material;
};

static_assert(CHittable<Sphere>);

#endif
