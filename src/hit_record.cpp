#include <hit_record.hpp>

#include <ray.hpp>

void setFaceNormal(HitRecord &record, const Ray &ray,
                   const Vec3 &outwardNormal) noexcept {
    record.frontFace = dot(ray.direction(), outwardNormal) < 0;
    record.normal = record.frontFace ? outwardNormal : -outwardNormal;
}
