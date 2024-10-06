#pragma once

#include <algorithm>
#include <memory>

#include "AABB.hpp"
#include "Hittable.hpp"
#include "Interval.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Sphere : public Hittable
{
public:
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> mat) : _center(center), _velocity(0, 0, 0), _isMoving(false), _radius(std::max(0.0, radius)), _mat(std::move(mat)) 
    {
        auto rvec = Vec3{radius, radius, radius};
        _bbox = AABB(_center - rvec, _center + rvec);
    }

    Sphere(const Point3& center, const Vec3& velocity, double radius, std::shared_ptr<Material> mat) : _center(center), _velocity(velocity), _isMoving(true), _radius(std::max(0.0, radius)), _mat(std::move(mat)) 
    {
        auto rvec = Vec3{radius, radius, radius};
        AABB box1(_curCenter(0) - rvec, _curCenter(0) + rvec);
        AABB box2(_curCenter(1) - rvec, _curCenter(1) + rvec);
        _bbox = AABB(box1, box2);
    }

    virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;
    virtual AABB boundingBox() const override { return _bbox; }

private:
    Point3 _center;
    Vec3 _velocity;
    bool _isMoving;
    double _radius;
    std::shared_ptr<Material> _mat;
    AABB _bbox;

    Point3 _curCenter(double time) const;
};
