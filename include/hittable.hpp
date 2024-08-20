#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <concepts>
#include <optional>

#include <aabb.hpp>
#include <hit_record.hpp>
#include <interval.hpp>
#include <ray.hpp>

namespace concepts {

template <typename H>
concept Hittable = requires(const Ray &ray, Interval rayT) {
    {
        std::declval<const H>().hit(ray, rayT)
    } -> std::same_as<std::optional<HitRecord>>;

    { std::declval<const H>().boundingBox() } -> std::same_as<const Aabb &>;
};

} // namespace concepts

#endif
