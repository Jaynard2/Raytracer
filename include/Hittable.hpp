#pragma once

#include "AABB.hpp"
#include "Interval.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include <memory>
#include <span>
#include <vector>

class Material;

struct HitRecord
{
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    bool frontFace;

    void setFaceNormal(const Ray& r, const Vec3& outwardNormal)
    {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
    virtual AABB boundingBox() const = 0;

private:

};

class HittableList : public Hittable
{
public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> obj) { add(obj); };

    void clear() { _objs.clear(); }
    void add(std::shared_ptr<Hittable> obj) 
    { 
        _objs.push_back(obj); 
        _bbox = AABB(_bbox, obj->boundingBox());
    }

    virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        HitRecord tmpRec;
        bool hit = false;
        auto closest = rayT.max();

        for(const auto& obj : _objs)
        {
            if(obj->hit(r, { rayT.min(), closest }, tmpRec))
            {
                hit = true;
                closest = tmpRec.t;
                rec = tmpRec;
            }
        }

        return hit;
    }

    virtual AABB boundingBox() const override { return _bbox; }

    std::span<std::shared_ptr<Hittable>> getObjs() { return _objs; }
    std::span<const std::shared_ptr<Hittable>> getObjs() const { return _objs; }

private:
    std::vector<std::shared_ptr<Hittable>> _objs;
    AABB _bbox;
};
