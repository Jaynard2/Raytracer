#pragma once

#include <algorithm>
#include <iterator>

#include "Interval.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class AABB
{
public:
    Interval x, y, z;

    constexpr AABB() = default;
    constexpr AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) { }
    constexpr AABB(const Point3& a, const Point3& b)
    {
        x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
    }
    constexpr AABB(const AABB& box1, const AABB& box2)
    {
        x = Interval(box1.x, box2.x);
        y = Interval(box1.y, box2.y);
        z = Interval(box1.z, box2.z);
    }

    const Interval& axisInterval(std::size_t n) const
    {
        if(n == 1) return y;
        if(n == 2) return z;
        return x;
    }

    bool hit(const Ray& r, Interval rayT) const
    {
        const Point3& rayOrig = r.origin();
        const Vec3& rayDir = r.direction();

        for(std::size_t axis = 0; axis < 3; ++axis)
        {
            const Interval& ax = axisInterval(axis);
            const double adinv = 1.0 / rayDir[axis];

            auto t0 = (ax.min() - rayOrig[axis]) * adinv;
            auto t1 = (ax.max() - rayOrig[axis]) * adinv;

            if(t0 < t1)
            {
                if(t0 > rayT.min()) rayT.min() = t0;
                if(t1 < rayT.max()) rayT.max() = t1;
            }
            else
            {
                if(t1 > rayT.min()) rayT.min() = t1;
                if(t0 < rayT.max()) rayT.max() = t0;
            }
        }

        if(rayT.max() <= rayT.min())
            return false;

        return true;
    }

    int longestAxis() const
    {
        const auto r = {x.size(), y.size(), z.size()};
        return std::distance(r.begin(), std::ranges::max_element(r));
    }

    static constexpr AABB empty()    { return AABB(EmptyInterval, EmptyInterval, EmptyInterval); }
    static constexpr AABB universe() { return AABB(UniverseInterval, UniverseInterval, UniverseInterval); }

private:

};

