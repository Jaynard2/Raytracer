#pragma once

#include <cmath>

class Interval
{
public:
    constexpr Interval() : _min(-INFINITY), _max(INFINITY) {}
    constexpr Interval(double min, double max) : _min(min), _max(max) {}
    constexpr Interval(const Interval& a, const Interval& b)
    {
        _min = a._min <= b._min ? a._min : b._min;
        _max = a._max >= b._max ? a._max : b._max;
    }

    constexpr double size() const { return _max - _min; }
    constexpr bool contains(double x) const { return _min <= x && _max >= x; }
    constexpr bool surrounds(double x) const { return _min < x && _max > x; }

    constexpr double max() const { return _max; }
    constexpr double min() const { return _min; }

    constexpr double& max() { return _max; }
    constexpr double& min() { return _min; }

    constexpr double clamp(double x) const
    {
        if(x < _min) return _min;
        if(x > _max) return _max;
        return x;
    }

    constexpr Interval expand(double delta) const
    {
        auto padding = delta / 2;
        return Interval{_min - padding, _max + padding};
    }


private:
    double _min, _max;
};

constexpr Interval EmptyInterval{INFINITY, -INFINITY};
constexpr Interval UniverseInterval{-INFINITY, INFINITY};
