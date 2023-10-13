#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <hittable.hpp>
#include <vec3.hpp>

class Sphere : public Hittable<Sphere> {
  public:
    explicit Sphere(Point3 center, double radius) noexcept;

    auto hit(const Ray &ray, double rayTmin, double rayTmax,
             HitRecord &record) const noexcept -> bool;

  private:
    Point3 m_center;
    double m_radius;
};

#endif
