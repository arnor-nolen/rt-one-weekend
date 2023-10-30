#include <hittable_list.hpp>

#include <algorithm>

#include <interval.hpp>

void HittableList::clear() {
    std::apply([](auto &...elems) { (elems.clear(), ...); }, m_objects);
}

auto HittableList::hit(const Ray &ray, Interval rayT) const
    -> std::optional<HitRecord> {

    auto record = std::optional<HitRecord>{};
    auto closestSoFar = rayT.getMax();

    const auto vectorLambda = [&](const auto &object) {
        auto tempRecord =
            object.hit(ray, Interval{rayT.getMin(), closestSoFar});
        if (tempRecord) {
            closestSoFar = tempRecord->time;
            record = *tempRecord;
        }
    };

    const auto tupleLambda = [&](const auto &...tuples) {
        (std::ranges::for_each(tuples, vectorLambda), ...);
    };

    std::apply(tupleLambda, m_objects);

    return record;
}
