#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <optional>

#include <hit_record.hpp>
#include <ray.hpp>

template <typename T>
class Hittable {
  public:
    [[nodiscard]]
    auto hit(const Ray &ray, double rayTmin, double rayTmax) const noexcept
        -> std::optional<HitRecord> {
        return underlying().hit(ray, rayTmin, rayTmax);
    }

    [[nodiscard]]
    auto underlying() noexcept -> T & {
        return static_cast<T &>(*this);
    }

    [[nodiscard]]
    auto underlying() const noexcept -> const T & {
        return static_cast<const T &>(*this);
    }

  private:
    explicit Hittable() noexcept = default;
    friend T;
};

#endif
