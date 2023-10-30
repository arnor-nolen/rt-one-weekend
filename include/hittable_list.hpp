#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <hittable.hpp>

#include <tuple>
#include <utility>
#include <vector>

#include <sphere.hpp>

class Interval;

class HittableList {
  public:
    explicit HittableList() = default;

    void clear();

    template <CHittable T, typename... Args>
    void add(Args &&...args) {
        std::get<std::vector<T>>(m_objects).emplace_back(
            std::forward<Args>(args)...);
    }

    [[nodiscard]]
    auto hit(const Ray &ray, Interval rayT) const -> std::optional<HitRecord>;

  private:
    using HittableTuple = std::tuple<std::vector<Sphere>>;

    HittableTuple m_objects{};
};

static_assert(CHittable<HittableList>);

#endif
