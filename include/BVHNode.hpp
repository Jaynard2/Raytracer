#pragma once

#include <cstddef>
#include <memory>
#include <span>

#include "AABB.hpp"
#include "Hittable.hpp"
#include "Interval.hpp"
#include "Ray.hpp"

class BVHNode : public Hittable
{
public:
    BVHNode(HittableList list) : BVHNode(list.getObjs(), 0, list.getObjs().size()) {}

    BVHNode(std::span<std::shared_ptr<Hittable>> objs, std::size_t start, std::size_t end);

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
    AABB boundingBox() const override { return _bbox; }

    static bool boxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axisIndex);
    static bool boxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool boxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool boxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);

  private:
    std::shared_ptr<Hittable> _left;
    std::shared_ptr<Hittable> _right;
    AABB _bbox;

};
