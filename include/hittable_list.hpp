#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <bvh.hpp>
#include <hittable.hpp>
#include <sphere.hpp>

#include <tuple>
#include <vector>

class Interval;
class Bvh;

class HittableList
{
  public:
    using HittableTuple = std::tuple<std::vector<Sphere>>;

    explicit HittableList() noexcept = default;
    explicit HittableList(concepts::Hittable auto &hittable) noexcept
    {
        add(std::forward(hittable));
    }

    void clear();

    template <concepts::Hittable T, typename... Args>
    void add(Args &&...args)
    {
        const auto &newHittable
            = std::get<std::vector<T>>(m_objects).emplace_back(
                std::forward<Args>(args)...);
        m_boundingBox = Aabb{m_boundingBox, newHittable.boundingBox()};
    }

    [[nodiscard]]
    auto hit(const Ray &ray, Interval rayT) const -> std::optional<HitRecord>;

    [[nodiscard]]
    auto boundingBox() const noexcept -> const Aabb &;

    [[nodiscard]]
    auto getObjects() const noexcept -> const HittableTuple &;

    [[nodiscard]]
    auto getObjects() noexcept -> HittableTuple &;

    void updateBvh() noexcept;

  private:
    HittableTuple m_objects;
    Bvh m_bvh;
    Aabb m_boundingBox;
};

static_assert(concepts::Hittable<HittableList>);

#endif
