#include <ray.hpp>

Ray::Ray(const Point3 &origin, const Vec3 &direction) noexcept
    : m_origin{origin}
    , m_direction{direction}
{}

Ray::Ray(const Point3 &origin, const Vec3 &direction, double time) noexcept
    : m_origin{origin}
    , m_direction{direction}
    , m_time{time}
{}

auto Ray::origin() const noexcept -> Point3 { return m_origin; }

auto Ray::direction() const noexcept -> Vec3 { return m_direction; }

auto Ray::at(double time) const noexcept -> Point3
{
    return m_origin + time * m_direction;
}

auto Ray::time() const noexcept -> double { return m_time; }
