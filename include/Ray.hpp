#pragma once
#include "Vec3.hpp"

class Ray
{
public:
    constexpr Ray() {}
    constexpr Ray(const Point3& origin, const Vec3& dir, double time) : _origin(origin), _dir(dir), _tm(time) {}

    constexpr const Point3& origin() const { return _origin; }
    constexpr const Vec3& direction() const { return _dir; }
    constexpr double time() const { return _tm; }

    constexpr Point3 at(double t) const
    {
        return _origin + t * _dir;
    }

private:
    Point3 _origin;
    Vec3 _dir;
    double _tm;

};
