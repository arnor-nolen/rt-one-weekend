#include <material.hpp>

#include <hit_record.hpp>
#include <ray.hpp>

Lambertian::Lambertian(const Color &color) noexcept : m_albedo{color} {}

auto Lambertian::scatter(const Ray & /* rayIn */, const HitRecord &record) const
    -> std::optional<ScatterInfo> {
    auto scatterDirection = record.normal + randomUnitVector();

    if (scatterDirection.nearZero()) {
        scatterDirection = record.normal;
    }

    return ScatterInfo{m_albedo, Ray{record.point, scatterDirection}};
}

Metal::Metal(const Color &color, double fuzz) noexcept
    : m_albedo{color}, m_fuzz{fuzz} {}

auto Metal::scatter(const Ray &rayIn, const HitRecord &record) const
    -> std::optional<ScatterInfo> {

    const auto reflected =
        reflect(unitVector(rayIn.direction()), record.normal);

    const auto scattered =
        Ray{record.point, reflected + m_fuzz * randomUnitVector()};

    if (dot(scattered.direction(), record.normal) <= 0) {
        return std::nullopt;
    }

    return ScatterInfo{m_albedo, scattered};
}
