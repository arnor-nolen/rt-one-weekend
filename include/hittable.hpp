#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <aabb.hpp>
#include <hit_record.hpp>
#include <interval.hpp>
#include <ray.hpp>

#include <concepts>
#include <optional>

namespace concepts
{

template <typename H>
concept Hittable = requires(const H hittable, const Ray &ray, Interval rayT) {
    {
        hittable.hit(ray, rayT)
    } -> std::same_as<std::optional<HitRecord>>;

    {
        hittable.boundingBox()
    } -> std::same_as<const Aabb &>;
};

} // namespace concepts

#endif
