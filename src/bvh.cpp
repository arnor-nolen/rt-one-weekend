#include <bvh.hpp>

#include <hittable_list.hpp>

#include <algorithm>
#include <cassert>
#include <stack>

namespace
{

struct ObjectRange
{
    size_t start;
    size_t end;
};

} // namespace

Bvh::Bvh(HittableList &list) noexcept
    : Bvh{ list.getObjects(),
           0,
           std::get<std::vector<Sphere>>(list.getObjects()).size() }
{
}

Bvh::Bvh(std::tuple<std::vector<Sphere>> &objects,
         size_t start,
         size_t end) noexcept
{
    initializeNodes(objects, start, end);
}

auto Bvh::hit(const Ray &ray, Interval rayT) const -> std::optional<HitRecord>
{
    assert(!m_nodes.empty());

    const auto *currentNode = &m_nodes.front();

    auto stack = std::stack<const BvhNode *, std::vector<const BvhNode *>>{};
    stack.push(currentNode);

    auto hitClosest = std::optional<HitRecord>{};
    auto rayTBeforeHit = rayT;

    while (!stack.empty())
    {
        while (!currentNode->m_sphere
               && currentNode->boundingBox().hit(ray, rayTBeforeHit))
        {

            stack.push(currentNode);
            currentNode = &m_nodes[currentNode->m_childIndexLeft];
        }

        if (currentNode->m_sphere)
        {
            const auto hitCurrent
                = currentNode->m_sphere->hit(ray, rayTBeforeHit);

            if (hitCurrent)
            {
                rayTBeforeHit = Interval{ rayT.getMin(), hitCurrent->time };
                hitClosest = hitCurrent;
            }
        }

        currentNode = stack.top();
        stack.pop();

        currentNode = &m_nodes[currentNode->m_childIndexRight];
    }

    return hitClosest;
}

auto Bvh::boundingBox() const noexcept -> const Aabb &
{
    return m_nodes.back().boundingBox();
}

void Bvh::initializeNodes(std::tuple<std::vector<Sphere>> &objects,
                          size_t start,
                          size_t end) noexcept
{

    auto &spheres = std::get<std::vector<Sphere>>(objects);

    auto objectRange = std::vector<ObjectRange>{};

    const size_t maxNodes = (end - start) * 2;

    m_nodes.reserve(maxNodes);
    objectRange.reserve(maxNodes);

    size_t currentNodeIndex = m_nodes.size();
    assert(currentNodeIndex == 0u);

    m_nodes.emplace_back();
    objectRange.emplace_back(start, end);

    auto stack = std::stack<size_t, std::vector<size_t>>{};
    stack.push(currentNodeIndex);

    while (!stack.empty())
    {
        while (!m_nodes[currentNodeIndex].m_sphere)
        {

            auto &currentNode = m_nodes[currentNodeIndex];

            const auto [objectStart, objectEnd] = objectRange[currentNodeIndex];

            size_t objectCount = objectEnd - objectStart;

            for (size_t objectIndex = objectStart; objectIndex < objectEnd;
                 ++objectIndex)
            {
                currentNode.m_boundingBox
                    = Aabb{ currentNode.m_boundingBox,
                            spheres[objectIndex].boundingBox() };
            }

            const Axis axis = currentNode.m_boundingBox.longestAxis();

            if (objectCount == 1)
            {
                currentNode.m_sphere = &spheres[objectStart];
                continue;
            }

            std::sort(std::begin(spheres) + static_cast<ptrdiff_t>(objectStart),
                      std::begin(spheres) + static_cast<ptrdiff_t>(objectEnd),
                      [&](const auto &elem1, const auto &elem2)
                      { return BvhNode::boxCompare(elem1, elem2, axis); });

            const auto mid = objectStart + objectCount / 2;

            currentNode.m_childIndexLeft = m_nodes.size();
            m_nodes.emplace_back();
            objectRange.emplace_back(objectStart, mid);

            currentNode.m_childIndexRight = m_nodes.size();
            m_nodes.emplace_back();
            objectRange.emplace_back(mid, objectEnd);

            stack.push(currentNodeIndex);

            currentNodeIndex = currentNode.m_childIndexLeft;
        }

        currentNodeIndex = stack.top();
        stack.pop();

        currentNodeIndex = m_nodes[currentNodeIndex].m_childIndexRight;
    }
}

auto BvhNode::boundingBox() const noexcept -> const Aabb &
{
    return m_boundingBox;
}

auto BvhNode::boxCompare(const Sphere &hittableA,
                         const Sphere &hittableB,
                         Axis axis) noexcept -> bool
{
    const auto aAxisInterval = hittableA.boundingBox().axisInterval(axis);
    const auto bAxisInterval = hittableB.boundingBox().axisInterval(axis);

    return aAxisInterval.getMin() < bAxisInterval.getMin();
}

auto Bvh::getNodes() const noexcept -> const std::vector<BvhNode> &
{
    return m_nodes;
}

auto Bvh::getNodes() noexcept -> std::vector<BvhNode> &
{
    return m_nodes;
}
