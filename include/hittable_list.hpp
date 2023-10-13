#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <hittable.hpp>

#include <memory>
#include <utility>
#include <variant>
#include <vector>

#include <sphere.hpp>

class HittableList : public Hittable<HittableList> {
  public:
    explicit HittableList() = default;

    template <typename T>
    explicit HittableList(std::shared_ptr<T> object) {
        add(std::move(object));
    }

    void clear();

    template <typename T>
    void add(std::shared_ptr<T> object) {
        m_objects.push_back(std::move(object));
    }

    [[nodiscard]]
    auto hit(const Ray &ray, double rayTmin, double rayTmax) const noexcept
        -> std::optional<HitRecord>;

  private:
    using HittableVariant =
        std::variant<std::shared_ptr<HittableList>, std::shared_ptr<Sphere>>;

    std::vector<HittableVariant> m_objects{};
};

#endif
