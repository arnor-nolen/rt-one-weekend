#include <sphere.hpp>

#include <interval.hpp>

Sphere::Sphere(const Point3 &center, double radius,
               MaterialVariant material) noexcept
    : m_center{center}, m_radius{radius}, m_material{material} {};

auto Sphere::hit(const Ray &ray, Interval rayT) const noexcept
    -> std::optional<HitRecord> {
    auto vec = ray.origin() - m_center;

    auto quadA = ray.direction().lengthSquared();
    auto quadH = dot(vec, ray.direction());
    auto quadC = vec.lengthSquared() - m_radius * m_radius;

    auto discriminant = quadH * quadH - quadA * quadC;

    if (discriminant < 0) {
        return std::nullopt;
    }

    auto sqrtD = std::sqrt(discriminant);

    auto root = (-quadH - sqrtD) / quadA;
    if (!rayT.surrounds(root)) {
        root = (-quadH + sqrtD) / quadA;
        if (!rayT.surrounds(root)) {
            return std::nullopt;
        }
    }

    HitRecord record = {
        .point = ray.at(root), .material = m_material, .time = root};
    setFaceNormal(record, ray, (record.point - m_center) / m_radius);

    return record;
}
