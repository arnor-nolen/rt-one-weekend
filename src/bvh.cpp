#include <bvh.hpp>

#include <algorithm>

#include <hittable_list.hpp>

Bvh::Bvh(HittableList &list) noexcept
    : Bvh{list.getObjects(), 0,
          std::get<std::vector<Sphere>>(list.getObjects()).size()} {}

Bvh::Bvh(std::tuple<std::vector<Sphere>> &objects, size_t start,
         size_t end) noexcept {
    auto bvhNode = BvhNode{this, objects, start, end};
    m_nodes.push_back(std::move(bvhNode));
}

Bvh::Bvh(Bvh &&other) noexcept : Bvh{} { swap(other); }

auto Bvh::operator=(Bvh &&other) noexcept -> Bvh & {
    swap(other);
    return *this;
}

void Bvh::swap(Bvh &other) noexcept {
    std::swap(m_nodes, other.m_nodes);
    for (auto &elem : m_nodes) {
        elem.m_bvh = this;
    }
}

auto Bvh::hit(const Ray &ray, Interval rayT) const -> std::optional<HitRecord> {
    return m_nodes.back().hit(ray, rayT);
}

auto Bvh::boundingBox() const noexcept -> const Aabb & {
    return m_nodes.back().boundingBox();
}

BvhNode::BvhNode(Bvh *bvh, std::tuple<std::vector<Sphere>> &objects,
                 size_t start, size_t end) noexcept
    : m_boundingBox{Aabb::s_empty}, m_bvh{bvh} {

    const size_t objectSpan = end - start;

    auto &spheres = std::get<std::vector<Sphere>>(objects);

    for (size_t objectIndex = start; objectIndex < end; ++objectIndex) {
        m_boundingBox = Aabb{m_boundingBox, spheres[objectIndex].boundingBox()};
    }

    const int axis = m_boundingBox.longestAxis();

    const auto comparator = (axis == 0)   ? boxXCompare
                            : (axis == 1) ? boxYCompare
                                          : boxZCompare;

    switch (objectSpan) {
    case 1: {
        m_leftSphere = &spheres[start];
        m_rightSphere = &spheres[start];
        break;
    }
    case 2: {
        m_leftSphere = &spheres[start];
        m_rightSphere = &spheres[start + 1];
        break;
    }
    default: {
        std::sort(std::begin(spheres) + static_cast<int64_t>(start),
                  std::begin(spheres) + static_cast<int64_t>(end), comparator);

        const auto mid = start + objectSpan / 2;

        auto leftNode = BvhNode{m_bvh, objects, start, mid};
        m_childIndexLeft = m_bvh->getNodes().size();
        m_bvh->getNodes().push_back(std::move(leftNode));

        auto rightNode = BvhNode{m_bvh, objects, mid, end};
        m_childIndexRight = m_bvh->getNodes().size();
        m_bvh->getNodes().push_back(std::move(rightNode));

        break;
    }
    }
}

auto BvhNode::hit(const Ray &ray,
                  Interval rayT) const -> std::optional<HitRecord> {
    if (!m_boundingBox.hit(ray, rayT)) {
        return std::nullopt;
    }

    if (m_leftSphere && m_rightSphere) {
        const auto hitLeft = m_leftSphere->hit(ray, rayT);

        const auto hitRight = m_rightSphere->hit(
            ray,
            Interval{rayT.getMin(), hitLeft ? hitLeft->time : rayT.getMax()});

        if (!hitLeft && !hitRight) {
            return std::nullopt;
        }

        return hitRight ? hitRight : hitLeft;
    }

    const auto &leftBvh = m_bvh->getNodes()[m_childIndexLeft];
    const auto hitLeft = leftBvh.hit(ray, rayT);

    const auto &rightBvh = m_bvh->getNodes()[m_childIndexRight];
    const auto hitRight = rightBvh.hit(
        ray, Interval{rayT.getMin(), hitLeft ? hitLeft->time : rayT.getMax()});

    if (!hitLeft && !hitRight) {
        return std::nullopt;
    }

    return hitRight ? hitRight : hitLeft;
}

auto BvhNode::boundingBox() const noexcept -> const Aabb & {
    return m_boundingBox;
}

auto BvhNode::boxCompare(const Sphere &hittableA, const Sphere &hittableB,
                         int axisIndex) noexcept -> bool {
    const auto aAxisInterval = hittableA.boundingBox().axisInterval(axisIndex);
    const auto bAxisInterval = hittableB.boundingBox().axisInterval(axisIndex);

    return aAxisInterval.getMin() < bAxisInterval.getMin();
}

auto BvhNode::boxXCompare(const Sphere &hittableA,
                          const Sphere &hittableB) noexcept -> bool {
    return boxCompare(hittableA, hittableB, 0);
}

auto BvhNode::boxYCompare(const Sphere &hittableA,
                          const Sphere &hittableB) noexcept -> bool {
    return boxCompare(hittableA, hittableB, 1);
}

auto BvhNode::boxZCompare(const Sphere &hittableA,
                          const Sphere &hittableB) noexcept -> bool {
    return boxCompare(hittableA, hittableB, 2);
}

auto Bvh::getNodes() const noexcept -> const std::vector<BvhNode> & {
    return m_nodes;
}

auto Bvh::getNodes() noexcept -> std::vector<BvhNode> & { return m_nodes; }
