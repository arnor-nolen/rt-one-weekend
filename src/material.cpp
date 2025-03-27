#include <material.hpp>

#include <hit_record.hpp>
#include <ray.hpp>

Lambertian::Lambertian(const Color &color) noexcept
    : m_albedo{ color }
{
}

auto Lambertian::scatter(const Ray &rayIn, const HitRecord &record) const
    -> std::optional<ScatterInfo>
{
    auto scatterDirection = record.normal + randomUnitVector();

    if (scatterDirection.nearZero())
    {
        scatterDirection = record.normal;
    }

    return ScatterInfo{ .attenuation = m_albedo,
                        .scattered = Ray{
                            record.point,
                            scatterDirection,
                            rayIn.time(),
                        } };
}

Metal::Metal(const Color &color, double fuzz) noexcept
    : m_albedo{ color }
    , m_fuzz{ fuzz }
{
}

auto Metal::scatter(const Ray &rayIn, const HitRecord &record) const
    -> std::optional<ScatterInfo>
{

    const auto reflected
        = reflect(unitVector(rayIn.direction()), record.normal);

    const auto scattered = Ray{ record.point,
                                reflected + m_fuzz * randomUnitVector(),
                                rayIn.time() };

    if (dot(scattered.direction(), record.normal) <= 0)
    {
        return std::nullopt;
    }

    return ScatterInfo{
        .attenuation = m_albedo,
        .scattered = scattered,
    };
}

Dielectric::Dielectric(double indexOfRefraction) noexcept
    : m_indexOfRefraction{ indexOfRefraction }
{
}

auto Dielectric::scatter(const Ray &rayIn, const HitRecord &record) const
    -> std::optional<ScatterInfo>
{

    auto refractionRatio
        = record.frontFace ? 1.0 / m_indexOfRefraction : m_indexOfRefraction;

    auto unitDirection = unitVector(rayIn.direction());
    auto cosTheta = std::fmin(dot(-unitDirection, record.normal), 1.0);
    auto sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    auto cannotRefract = refractionRatio * sinTheta > 1.0;
    auto direction
        = cannotRefract
               || reflectance(cosTheta, refractionRatio) > randomDouble()
            ? reflect(unitDirection, record.normal)
            : refract(unitDirection, record.normal, refractionRatio);

    return ScatterInfo{
        .attenuation = Color{ 1.0, 1.0, 1.0 },
        .scattered = Ray{ record.point, direction, rayIn.time() },
    };
}

auto Dielectric::reflectance(double cosine, double refIdx) -> double
{
    auto reflectionCoeff = (1 - refIdx) / (1 + refIdx);
    reflectionCoeff *= reflectionCoeff;
    return reflectionCoeff + (1 - reflectionCoeff) * std::pow((1 - cosine), 5);
}
