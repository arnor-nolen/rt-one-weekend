#include <hittable_list.hpp>

void HittableList::clear() { m_objects.clear(); }

auto HittableList::hit(const Ray &ray, double rayTmin, double rayTmax,
                       HitRecord &record) const noexcept -> bool {
    HitRecord tempRecord;

    bool hitAnything = false;
    auto closestSoFar = rayTmax;

    for (const auto &elem : m_objects) {
        std::visit(
            [&](const auto &object) {
                if (object->hit(ray, rayTmin, closestSoFar, tempRecord)) {
                    hitAnything = true;
                    closestSoFar = tempRecord.time;
                    record = tempRecord;
                }
            },
            elem);
    }

    return hitAnything;
}
