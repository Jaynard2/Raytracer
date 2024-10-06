#include "Vec3.hpp"
#include "Interval.hpp"
#include "fmt/base.h"
#include <algorithm>
#include <cmath>

Vec3& Vec3::operator+=(const Vec3& v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

Vec3& Vec3::operator*=(double t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;

    return *this;
}

Vec3& Vec3::operator/=(double t)
{
    return *this *= (1 / t);
}

double Vec3::length() const
{
    return std::sqrt(lengthSquared());
}

double Vec3::lengthSquared() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}
auto fmt::formatter<Vec3>::format(Vec3 v, format_context &ctx) const
    -> format_context::iterator 
{
    if(isColor)
    {
        constexpr Interval intensity(0, 0.999);
        int r = 256 * intensity.clamp(v.x());
        int g = 256 * intensity.clamp(v.y());
        int b = 256 * intensity.clamp(v.z());

        return format_to(ctx.out(), "{} {} {}", r, g, b);
    }

    return format_to(ctx.out(), "[{}, {}, {}]", v.e[0], v.e[1], v.e[2]);
}
Vec3 Vec3::random(double min, double max) 
{
  return { randomDouble(min, max), randomDouble(min, max), randomDouble(min, max) };
}
bool Vec3::nearZero() const 
{
    constexpr auto s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
}
