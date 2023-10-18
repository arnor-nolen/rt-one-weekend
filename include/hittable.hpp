#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <concepts>
#include <optional>
#include <utility>

#include <hit_record.hpp>
#include <interval.hpp>
#include <ray.hpp>

template <typename H>
concept CHittable = requires(const Ray &ray, Interval rayT) {
    {
        std::declval<const H>().hit(ray, rayT)
    } -> std::same_as<std::optional<HitRecord>>;
};

#endif
