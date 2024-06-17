#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <hittable.hpp>
#include <material.hpp>
#include <optional>
#include <vec3.hpp>

class Interval;

class Sphere {
  public:
    explicit Sphere(const Point3 &center, double radius,
                    MaterialVariant material) noexcept;
    explicit Sphere(const Point3 &center1, const Point3 &center2, double radius,
                    MaterialVariant material) noexcept;

    [[nodiscard]]
    auto hit(const Ray &ray, Interval rayT) const noexcept
        -> std::optional<HitRecord>;

    [[nodiscard]]
    auto sphereCenter(double time) const noexcept -> Point3;

  private:
    Point3 m_center1;
    Point3 m_centerVec;
    double m_radius;
    MaterialVariant m_material;
    bool m_isMoving{false};
};

static_assert(CHittable<Sphere>);

#endif
