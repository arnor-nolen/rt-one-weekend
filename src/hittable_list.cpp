#include <hittable_list.hpp>

void HittableList::clear() { m_objects.clear(); }

auto HittableList::hit(const Ray &ray, double rayTmin,
                       double rayTmax) const noexcept
    -> std::optional<HitRecord> {
    auto record = std::optional<HitRecord>{};
    auto closestSoFar = rayTmax;

    for (const auto &elem : m_objects) {
        std::visit(
            [&](const auto &object) {
                auto tempRecord = object->hit(ray, rayTmin, closestSoFar);
                if (tempRecord) {
                    closestSoFar = tempRecord->time;
                    record = *tempRecord;
                }
            },
            elem);
    }

    return record;
}
