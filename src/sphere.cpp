#include <sphere.hpp>

#include <interval.hpp>

Sphere::Sphere(const Point3 &center, double radius,
               MaterialVariant material) noexcept
    : m_center1{center}, m_radius{fmax(0, radius)}, m_material{material} {};

Sphere::Sphere(const Point3 &center1, const Point3 &center2, double radius,
               MaterialVariant material) noexcept
    : m_center1{center1}, m_centerVec{center2 - center1},
      m_radius{fmax(0, radius)}, m_material{material}, m_isMoving{true} {};

auto Sphere::hit(const Ray &ray, Interval rayT) const noexcept
    -> std::optional<HitRecord> {
    const auto center = m_isMoving ? sphereCenter(ray.time()) : m_center1;
    const auto vec = ray.origin() - center;

    const auto quadA = ray.direction().lengthSquared();
    const auto quadH = dot(vec, ray.direction());
    const auto quadC = vec.lengthSquared() - m_radius * m_radius;

    const auto discriminant = quadH * quadH - quadA * quadC;

    if (discriminant < 0) {
        return std::nullopt;
    }

    const auto sqrtD = std::sqrt(discriminant);

    auto root = (-quadH - sqrtD) / quadA;
    if (!rayT.surrounds(root)) {
        root = (-quadH + sqrtD) / quadA;
        if (!rayT.surrounds(root)) {
            return std::nullopt;
        }
    }

    HitRecord record = {
        .point = ray.at(root), .material = m_material, .time = root};
    setFaceNormal(record, ray, (record.point - center) / m_radius);

    return record;
}

auto Sphere::sphereCenter(double time) const noexcept -> Point3 {
    return m_center1 + time * m_centerVec;
}
