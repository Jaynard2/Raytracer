#include "BVHNode.hpp"
#include "AABB.hpp"
#include "Interval.hpp"
#include <algorithm>
#include <memory>
#include <ranges>

BVHNode::BVHNode(std::span<std::shared_ptr<Hittable>> objs, std::size_t start, std::size_t end)
{
    
    _bbox = AABB::empty();
    for(auto i : std::views::iota(start, end))
        _bbox = AABB(_bbox, objs[i]->boundingBox());
    
    int axis = _bbox.longestAxis();
    auto compare = (axis == 0) ? boxXCompare
                 : (axis == 1) ? boxYCompare
                               : boxZCompare;

    std::size_t size = end - start;

    if(size == 1)
        _left = _right = objs[start];
    else if(size == 2)
    {
        _left = objs[start];
        _right = objs[start + 1];
    }
    else
    {
        std::sort(objs.begin() + start, objs.begin() + end, compare);

        auto mid = start + size / 2;
        _left = std::make_shared<BVHNode>(objs, start, mid);
        _right = std::make_shared<BVHNode>(objs, mid, end);
    }

}

bool BVHNode::hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    if(!_bbox.hit(r, ray_t))
        return false;

    bool hit = _left->hit(r, ray_t, rec);
    hit |= _right->hit(r, Interval(ray_t.min(), hit ? rec.t : ray_t.max()), rec);

    return hit;
}
bool BVHNode::boxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axisIndex) 
{
    auto aInterval = a->boundingBox().axisInterval(axisIndex);
    auto bInterval = b->boundingBox().axisInterval(axisIndex);
    return aInterval.min() < bInterval.min();
}
bool BVHNode::boxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) 
{
    return boxCompare(a, b, 0);
}

bool BVHNode::boxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) 
{
    return boxCompare(a, b, 1);
}

bool BVHNode::boxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) 
{
    return boxCompare(a, b, 2);
}
