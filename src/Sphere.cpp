#include "Sphere.hpp"
#include "Interval.hpp"
#include <cmath>

bool Sphere::hit(const Ray& r, Interval rayT, HitRecord& rec) const
{
    Vec3 oc = _curCenter(r.time()) - r.origin();
    auto a = r.direction().lengthSquared();
    auto h = dot(r.direction(), oc);
    auto c = oc.lengthSquared() - _radius * _radius;
    auto discriminant = h * h - a * c;
    
    if(discriminant < 0.0)
        return false;

    auto sqrtd = std::sqrt(discriminant);

    // Find nearest root in range
    auto root = (h - sqrtd) / a;
    if(!rayT.surrounds(root))
    {
        root = (h + sqrtd) / a;
        if(!rayT.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    auto normal = (rec.p - _center) / _radius;
    rec.setFaceNormal(r, normal);
    rec.mat = _mat;
    return true;
}
Point3 Sphere::_curCenter(double time) const 
{
    return _center + time * _velocity;
}
