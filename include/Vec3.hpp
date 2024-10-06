#pragma once

#include "fmt/base.h"
#include "rtweekend.hpp"
#include <array>
#include <cmath>
#include <cstddef>

#include <cstdlib>
#include <fmt/format.h>

class Vec3
{
public:
    std::array<double, 3> e;

    constexpr Vec3() : e{0, 0, 0} {}
    constexpr Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    constexpr double x() const { return e[0]; }
    constexpr double y() const { return e[1]; }
    constexpr double z() const { return e[2]; }

    constexpr Vec3 operator-() const { return Vec3{-e[0], -e[1], -e[2]}; }
    constexpr double operator[](std::size_t i) const { return e[i]; }
    constexpr double& operator[](std::size_t i) { return e[i]; }

    Vec3& operator+=(const Vec3& v);
    Vec3& operator*=(double t);
    Vec3& operator/=(double t);

    static Vec3 random(double min = 0.0, double max = 1.0);

    double length() const;
    double lengthSquared() const;
    bool nearZero() const;

  private:
};

using Point3 = Vec3;
using Color = Vec3;

template <>
struct fmt::formatter<Vec3>
{
  constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator
  {
    auto fmtEnd = std::ranges::find(ctx, '}');
    if(fmtEnd != ctx.end())
    {
        char rep = *ctx.begin();
        if(rep == 'c')
            isColor = true;
    }

    ctx.advance_to(std::next(ctx.begin()));
    return ctx.begin();
}

  auto format(Vec3 v, format_context &ctx) const -> format_context::iterator;

  bool isColor = false;
};

constexpr inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
    return { lhs.e[0] + rhs.e[0], lhs.e[1] + rhs.e[1], lhs.e[2] + rhs.e[2] };
}

constexpr inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
    return { lhs.e[0] - rhs.e[0], lhs.e[1] - rhs.e[1], lhs.e[2] - rhs.e[2] };
}

constexpr inline Vec3 operator*(const Vec3& lhs, const Vec3& rhs)
{
    return { lhs.e[0] * rhs.e[0], lhs.e[1] * rhs.e[1], lhs.e[2] * rhs.e[2] };
}

constexpr inline Vec3 operator*(double lhs, const Vec3& rhs)
{
    return { lhs * rhs.e[0], lhs * rhs.e[1], lhs * rhs.e[2] };
}

constexpr inline Vec3 operator*(const Vec3& rhs, double lhs)
{
    return lhs * rhs;
}

constexpr inline Vec3 operator/(const Vec3& lhs, double rhs)
{
    return lhs * (1 / rhs);
}

constexpr inline double dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.e[0] * rhs.e[0]
        + lhs.e[1] * rhs.e[1]
        + lhs.e[2] * rhs.e[2];
}

constexpr inline Vec3 cross(const Vec3& lhs, const Vec3& rhs)
{
    return {
        lhs.e[1] * rhs.e[2] - lhs.e[2] * rhs.e[1],
        lhs.e[2] * rhs.e[0] - lhs.e[0] * rhs.e[2],
        lhs.e[0] * rhs.e[1] - lhs.e[1] * rhs.e[0]
    };
}

constexpr inline Vec3 unitVector(const Vec3& v)
{
    return v / v.length();
}

inline Vec3 randomInUnitSphere()
{
    while(true)
    {
        auto p = Vec3::random(-1, 1);
        if(p.lengthSquared() < 1)
            return p;
    }
}

inline Vec3 randomUnitVector()
{
    return unitVector(randomInUnitSphere());
}

inline Vec3 randomOnHemisphere(const Vec3& normal)
{
    Vec3 unitSphere = randomInUnitSphere();
    if(dot(unitSphere, normal) > 0.0)
        return unitSphere;
    return -unitSphere;
}

inline Color linearToGamma(const Color& c)
{
    return {
        c.e[0] > 0 ? std::sqrt(c.e[0]) : 0,
        c.e[1] > 0 ? std::sqrt(c.e[1]) : 0,
        c.e[2] > 0 ? std::sqrt(c.e[2]) : 0
    };
}

inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etaiOverEtat)
{
    auto cosTheta = std::min(dot(-uv, n), 1.0);
    Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vec3 rOutParallel = -std::sqrt(std::abs(1.0 - rOutPerp.lengthSquared())) * n;

    return rOutPerp + rOutParallel;
}

inline Vec3 randomInUnitDisk()
{
    while(true)
    {
        Vec3 p{ randomDouble(-1, 1), randomDouble(-1, 1), 0 };
        if(p.lengthSquared() < 1)
            return p;
    }
}
