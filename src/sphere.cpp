#include <sphere.hpp>

Sphere::Sphere(Point3 center, double radius) noexcept
    : m_center{center}, m_radius{radius} {};

auto Sphere::hit(const Ray &ray, double rayTmin, double rayTmax) const noexcept
    -> std::optional<HitRecord> {
    auto vec = ray.origin() - m_center;

    auto quadA = ray.direction().length_squared();
    auto quadH = dot(vec, ray.direction());
    auto quadC = vec.length_squared() - m_radius * m_radius;

    auto discriminant = quadH * quadH - quadA * quadC;

    if (discriminant < 0) {
        return std::nullopt;
    }

    auto sqrtD = std::sqrt(discriminant);

    auto root = (-quadH - sqrtD) / quadA;
    if (root <= rayTmin || rayTmax <= root) {
        root = (-quadH + sqrtD) / quadA;
        if (root <= rayTmin || rayTmax <= root) {
            return std::nullopt;
        }
    }

    HitRecord record = {.point = ray.at(root), .time = root};
    setFaceNormal(record, ray, (record.point - m_center) / m_radius);

    return record;
}
