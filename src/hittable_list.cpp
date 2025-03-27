#include <hittable_list.hpp>

#include <bvh.hpp>
#include <interval.hpp>
#include <sphere.hpp>

void HittableList::clear()
{
    std::apply([](auto &...elems) { (elems.clear(), ...); }, m_objects);
}

auto HittableList::hit(const Ray &ray, Interval rayT) const
    -> std::optional<HitRecord>
{

    auto record = std::optional<HitRecord>{};
    auto closestSoFar = rayT.getMax();

    const auto vectorLambda = [&](const auto &object)
    {
        auto tempRecord
            = object.hit(ray, Interval{ rayT.getMin(), closestSoFar });
        if (tempRecord)
        {
            closestSoFar = tempRecord->time;
            record = tempRecord;
        }
    };

    vectorLambda(m_bvh);

    return record;
}

auto HittableList::boundingBox() const noexcept -> const Aabb &
{
    return m_boundingBox;
}

auto HittableList::getObjects() const noexcept -> const HittableTuple &
{
    return m_objects;
}

auto HittableList::getObjects() noexcept -> HittableTuple &
{
    return m_objects;
}

void HittableList::updateBvh() noexcept
{
    m_bvh = Bvh{ *this };
}
