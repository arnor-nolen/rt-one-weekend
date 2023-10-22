#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <concepts>
#include <optional>
#include <utility>
#include <variant>

#include <color.hpp>
#include <ray.hpp>

struct HitRecord;

struct ScatterInfo {
    Color attenuation;
    Ray scattered;
};

template <typename T>
concept CMaterial = requires(const Ray &rayIn, const HitRecord &record) {
    {
        std::declval<const T>().scatter(rayIn, record)
    } -> std::same_as<std::optional<ScatterInfo>>;
};

class Lambertian {
  public:
    explicit Lambertian(const Color &color) noexcept;

    [[nodiscard]]
    auto scatter(const Ray &rayIn, const HitRecord &record) const
        -> std::optional<ScatterInfo>;

  private:
    Color m_albedo;
};

static_assert(CMaterial<Lambertian>);

class Metal {
  public:
    explicit Metal(const Color &color, double fuzz) noexcept;

    [[nodiscard]]
    auto scatter(const Ray &rayIn, const HitRecord &record) const
        -> std::optional<ScatterInfo>;

  private:
    Color m_albedo;
    double m_fuzz;
};

static_assert(CMaterial<Metal>);

class Dielectric {
  public:
    explicit Dielectric(double indexOfRefraction) noexcept;

    [[nodiscard]]
    auto scatter(const Ray &rayIn, const HitRecord &record) const
        -> std::optional<ScatterInfo>;

  private:
    static auto reflectance(double cosine, double refIdx) -> double;

    double m_indexOfRefraction;
};

static_assert(CMaterial<Dielectric>);

using MaterialVariant = std::variant<Lambertian, Metal, Dielectric>;

#endif
