#include <bvh.hpp>

#include <algorithm>
// #include <bit>
#include <cassert>
#include <stack>

#include <hittable_list.hpp>

Bvh::Bvh(HittableList &list) noexcept
    : Bvh{list.getObjects(), 0,
          std::get<std::vector<Sphere>>(list.getObjects()).size()} {}

Bvh::Bvh(std::tuple<std::vector<Sphere>> &objects, size_t start,
         size_t end) noexcept {
    auto bvhNode = BvhNode{this, objects, start, end};
    m_nodes.push_back(bvhNode);
}

auto Bvh::hit(const Ray &ray, Interval rayT) const -> std::optional<HitRecord> {
    assert(!m_nodes.empty());

    const auto *currentNode = &m_nodes.back();

    // auto container = std::vector<const BvhNode *>{};
    // const auto treeDepth =
    // static_cast<size_t>(std::bit_width(m_nodes.size()));
    //
    // container.reserve(treeDepth);

    auto stack = std::stack<const BvhNode *, std::vector<const BvhNode *>>{};
    stack.push(currentNode);

    auto hitClosest = std::optional<HitRecord>{};
    auto rayTBeforeHit = rayT;

    while (!stack.empty()) {
        while (!currentNode->m_sphere &&
               currentNode->boundingBox().hit(ray, rayTBeforeHit)) {

            stack.push(currentNode);
            currentNode = &m_nodes[currentNode->m_childIndexLeft];
        }

        if (currentNode->m_sphere) {
            const auto hitCurrent =
                currentNode->m_sphere->hit(ray, rayTBeforeHit);

            if (hitCurrent) {
                rayTBeforeHit = Interval{rayT.getMin(), hitCurrent->time};
                hitClosest = hitCurrent;
            }
        }

        currentNode = stack.top();
        stack.pop();

        currentNode = &m_nodes[currentNode->m_childIndexRight];
    }

    return hitClosest;
}

auto Bvh::boundingBox() const noexcept -> const Aabb & {
    return m_nodes.back().boundingBox();
}

BvhNode::BvhNode(Bvh *bvh, std::tuple<std::vector<Sphere>> &objects,
                 size_t start, size_t end) noexcept
    : m_boundingBox{Aabb::s_empty} {

    const size_t objectSpan = end - start;

    assert(objectSpan != 0);

    auto &spheres = std::get<std::vector<Sphere>>(objects);

    for (size_t objectIndex = start; objectIndex < end; ++objectIndex) {
        m_boundingBox = Aabb{m_boundingBox, spheres[objectIndex].boundingBox()};
    }

    const int axis = m_boundingBox.longestAxis();

    const auto comparator = (axis == 0)   ? boxXCompare
                            : (axis == 1) ? boxYCompare
                                          : boxZCompare;

    if (objectSpan == 1) {
        m_sphere = &spheres[start];
        return;
    }

    std::sort(std::begin(spheres) + static_cast<ptrdiff_t>(start),
              std::begin(spheres) + static_cast<ptrdiff_t>(end), comparator);

    const auto mid = start + objectSpan / 2;

    auto leftNode = BvhNode{bvh, objects, start, mid};
    m_childIndexLeft = bvh->getNodes().size();
    bvh->getNodes().push_back(std::move(leftNode));

    auto rightNode = BvhNode{bvh, objects, mid, end};
    m_childIndexRight = bvh->getNodes().size();
    bvh->getNodes().push_back(std::move(rightNode));
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
