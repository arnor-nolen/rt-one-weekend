#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <concepts>
#include <optional>
#include <utility>

#include <hit_record.hpp>
#include <ray.hpp>

template <typename H>
concept CHittable = requires(const Ray &ray, double rayTmin, double rayTmax) {
    {
        std::declval<const H>().hit(ray, rayTmin, rayTmax)
    } noexcept -> std::same_as<std::optional<HitRecord>>;
};

// Equivalent CRTP code, for comparison.
//
// template <typename T>
// class Hittable {
//   public:
//     [[nodiscard]]
//     auto hit(const Ray &ray, double rayTmin, double rayTmax) const noexcept
//         -> std::optional<HitRecord> {
//         return underlying().hit(ray, rayTmin, rayTmax);
//     }
//
//     [[nodiscard]]
//     auto underlying() noexcept -> T & {
//         return static_cast<T &>(*this);
//     }
//
//     [[nodiscard]]
//     auto underlying() const noexcept -> const T & {
//         return static_cast<const T &>(*this);
//     }
//
//   private:
//     explicit Hittable() noexcept = default;
//     friend T;
// };

#endif
