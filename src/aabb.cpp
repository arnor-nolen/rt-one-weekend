#include <aabb.hpp>

#include <halt.hpp>

#include <algorithm>

const Aabb Aabb::s_empty
    = Aabb{Interval::empty(), Interval::empty(), Interval::empty()};

const Aabb Aabb::s_universe
    = Aabb{Interval::universe(), Interval::universe(), Interval::universe()};

Aabb::Aabb(const Interval &intervalX,
           const Interval &intervalY,
           const Interval &intervalZ) noexcept
    : m_x{intervalX}
    , m_y{intervalY}
    , m_z{intervalZ}
{}

Aabb::Aabb(const Point3 &pointA, const Point3 &pointB) noexcept
{
    m_x = Interval(std::minmax(pointA[0], pointB[0]));
    m_y = Interval(std::minmax(pointA[1], pointB[1]));
    m_z = Interval(std::minmax(pointA[2], pointB[2]));
}

Aabb::Aabb(const Aabb &box1, const Aabb &box2) noexcept
    : m_x{box1.getX(), box2.getX()}
    , m_y{box1.getY(), box2.getY()}
    , m_z{box1.getZ(), box2.getZ()}
{}

auto Aabb::axisInterval(Axis axis) const noexcept -> const Interval &
{
    switch (axis)
    {
    case Axis::X:
        return m_x;
    case Axis::Y:
        return m_y;
    case Axis::Z:
        return m_z;
    default:
        halt("Unknown enum value.");
    }
}

auto Aabb::hit(const Ray &ray, const Interval &rayT) const noexcept -> bool
{
    const Point3 &rayOrig = ray.origin();
    const Vec3 &rayDir = ray.direction();

    for (uint8_t axisIdx = 0; axisIdx < 3; ++axisIdx)
    {
        const Interval &axis = axisInterval(Axis{axisIdx});
        const double axisDirInv = 1.0 / rayDir[axisIdx];

        const auto [time0, time1]
            = std::minmax({(axis.getMin() - rayOrig[axisIdx]) * axisDirInv,
                           (axis.getMax() - rayOrig[axisIdx]) * axisDirInv});

        const auto newRayTime = Interval{std::max(rayT.getMin(), time0),
                                         std::min(rayT.getMax(), time1)};

        if (newRayTime.getMax() <= newRayTime.getMin())
        {
            return false;
        }
    }

    return true;
}

auto Aabb::getX() const noexcept -> Interval { return m_x; }

auto Aabb::getY() const noexcept -> Interval { return m_y; }

auto Aabb::getZ() const noexcept -> Interval { return m_z; }

auto Aabb::longestAxis() const noexcept -> Axis
{
    if (m_x.size() > m_y.size())
    {
        return m_x.size() > m_z.size() ? Axis{0u} : Axis{2u};
    }

    return m_y.size() > m_z.size() ? Axis{1u} : Axis{2u};
}
