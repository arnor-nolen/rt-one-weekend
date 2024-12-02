#ifndef BVH_HPP
#define BVH_HPP

#include <vector>

#include <aabb.hpp>
#include <hittable.hpp>
#include <sphere.hpp>

class Bvh;
class HittableList;

class BvhNode {
  public:
    explicit BvhNode() noexcept = default;

    [[nodiscard]]
    auto boundingBox() const noexcept -> const Aabb &;

  private:
    [[nodiscard]]
    static auto boxCompare(const Sphere &hittableA, const Sphere &hittableB,
                           int axisIndex) noexcept -> bool;

    [[nodiscard]]
    static auto boxXCompare(const Sphere &hittableA,
                            const Sphere &hittableB) noexcept -> bool;

    [[nodiscard]]
    static auto boxYCompare(const Sphere &hittableA,
                            const Sphere &hittableB) noexcept -> bool;

    [[nodiscard]]
    static auto boxZCompare(const Sphere &hittableA,
                            const Sphere &hittableB) noexcept -> bool;

    size_t m_childIndexLeft{std::numeric_limits<size_t>::max()};
    size_t m_childIndexRight{std::numeric_limits<size_t>::max()};

    const Sphere *m_sphere{nullptr};

    Aabb m_boundingBox{Aabb::s_empty};

    friend Bvh;
};

class Bvh {
  public:
    explicit Bvh() noexcept = default;
    explicit Bvh(HittableList &list) noexcept;
    explicit Bvh(std::tuple<std::vector<Sphere>> &objects, size_t start,
                 size_t end) noexcept;

    void initializeNodes(std::tuple<std::vector<Sphere>> &objects, size_t start,
                         size_t end) noexcept;

    [[nodiscard]]
    auto hit(const Ray &ray, Interval rayT) const -> std::optional<HitRecord>;

    [[nodiscard]]
    auto boundingBox() const noexcept -> const Aabb &;

    [[nodiscard]]
    auto getNodes() const noexcept -> const std::vector<BvhNode> &;

    [[nodiscard]]
    auto getNodes() noexcept -> std::vector<BvhNode> &;

  private:
    std::vector<BvhNode> m_nodes;
};

static_assert(concepts::Hittable<Bvh>);

#endif