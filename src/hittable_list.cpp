#include <hittable_list.hpp>

#include <interval.hpp>

void HittableList::clear() { m_objects.clear(); }

auto HittableList::hit(const Ray &ray, Interval rayT) const
    -> std::optional<HitRecord> {

    auto record = std::optional<HitRecord>{};
    auto closestSoFar = rayT.getMax();

    for (const auto &elem : m_objects) {
        std::visit(
            [&](const auto &object) {
                auto tempRecord =
                    object->hit(ray, Interval{rayT.getMin(), closestSoFar});
                if (tempRecord) {
                    closestSoFar = tempRecord->time;
                    record = *tempRecord;
                }
            },
            elem);
    }

    return record;
}
