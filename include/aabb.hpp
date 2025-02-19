#ifndef AABB_HPP
#define AABB_HPP

#include <axis.hpp>
#include <interval.hpp>
#include <ray.hpp>
#include <vec3.hpp>

class Aabb
{
public:
    static const Aabb s_empty;
    static const Aabb s_universe;

    explicit Aabb() noexcept = default;
    explicit Aabb(const Interval &intervalX,
                  const Interval &intervalY,
                  const Interval &intervalZ) noexcept;
    explicit Aabb(const Point3 &pointA, const Point3 &pointB) noexcept;
    explicit Aabb(const Aabb &box1, const Aabb &box2) noexcept;

    [[nodiscard]]
    auto axisInterval(Axis axis) const noexcept -> const Interval &;

    [[nodiscard]]
    auto hit(const Ray &ray, const Interval &rayT) const noexcept -> bool;

    [[nodiscard]]
    auto getX() const noexcept -> Interval;

    [[nodiscard]]
    auto getY() const noexcept -> Interval;

    [[nodiscard]]
    auto getZ() const noexcept -> Interval;

    [[nodiscard]]
    auto longestAxis() const noexcept -> Axis;

private:
    Interval m_x;
    Interval m_y;
    Interval m_z;
};
#endif
