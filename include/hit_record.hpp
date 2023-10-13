#ifndef HIT_RECORD_HPP
#define HIT_RECORD_HPP

#include <vec3.hpp>

class Ray;

struct HitRecord {
    Point3 point{};
    Vec3 normal{};
    double time{};
    bool frontFace{};
};

void setFaceNormal(HitRecord &record, const Ray &ray,
                   const Vec3 &outwardNormal) noexcept;

#endif
