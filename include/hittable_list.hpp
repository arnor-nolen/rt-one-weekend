#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <hittable.hpp>

#include <memory>
#include <utility>
#include <variant>
#include <vector>

#include <sphere.hpp>

class Interval;

class HittableList {
  public:
    explicit HittableList() = default;

    template <CHittable T>
    explicit HittableList(std::shared_ptr<T> object) {
        add(std::move(object));
    }

    void clear();

    template <CHittable T>
    void add(std::shared_ptr<T> object) {
        m_objects.push_back(std::move(object));
    }

    [[nodiscard]]
    auto hit(const Ray &ray, Interval rayT) const -> std::optional<HitRecord>;

  private:
    using HittableVariant =
        std::variant<std::shared_ptr<HittableList>, std::shared_ptr<Sphere>>;

    std::vector<HittableVariant> m_objects{};
};

static_assert(CHittable<HittableList>);

#endif
