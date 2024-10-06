#include "Camera.hpp"

#include <cmath>

#include <filesystem>
#include <ios>
#include <iostream>
#include <stdexcept>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <fmt/std.h>


#include "Hittable.hpp"
#include "Interval.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include "rtweekend.hpp"

Camera::Camera(std::filesystem::path outputFile) : _output(outputFile, std::ios_base::out)
{
    if(!_output.is_open())
        throw std::runtime_error(fmt::format("Failed to open output file at {}", outputFile));
}

void Camera::render(const Hittable &world) 
{
    _init();
    auto constexpr clearCode = "\33[2K";

    fmt::println(_output, "P3\n{} {}\n255", imageWidth, _imagHeight);
    std::vector<Color> screen;
    screen.reserve(imageWidth * _imagHeight);

    for(std::size_t j = 0; j < _imagHeight; ++j)
    {
        fmt::print(std::clog, "\r{}Scanlines remaining: {}", clearCode, _imagHeight - j);
        for(std::size_t i = 0; i < imageWidth; ++i)
        {
            Color pixColor{0, 0, 0};
            for(std::size_t sample = 0; sample < samplesPerPixel; ++sample)
            {
                Ray r = _getRay(i, j);
                pixColor += _rayColor(r, maxDepth, world);
            }
            pixColor *= _pixelSamplesScale;
            screen.push_back(linearToGamma(pixColor));
        }
    }

    fmt::println(_output, "{:c}", fmt::join(screen, "\n"));
    fmt::println(std::clog,"{}\rDone", clearCode);
}

Color Camera::_rayColor(const Ray& ray, std::size_t depth, const Hittable& world) const
{
    if(depth <= 0)
        return {0, 0, 0};

    HitRecord rec;
    if(world.hit(ray, Interval{0.001, INFINITY}, rec))
    {
        Ray scattered;
        Color attenuation;
        if(rec.mat->scatter(ray, rec, attenuation, scattered))
            return attenuation * _rayColor(scattered, depth - 1, world);
        return Color(0, 0, 0);
    }

    Vec3 unitDir = unitVector(ray.direction());
    auto a = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

void Camera::_init() 
{
    _imagHeight = imageWidth / aspectRatio < 1 ? 1 : imageWidth / aspectRatio;
    _pixelSamplesScale = 1.0 / samplesPerPixel;
    _center = lookFrom;

    auto theta = degreeToRadians(vfov);
    auto h = std::tan(theta / 2.0);
    double viewportHeight = 2 * h * focusDistance;
    double viewportWidth = viewportHeight * imageWidth / _imagHeight;

    _w = unitVector(lookFrom - lookat);
    _u = unitVector(cross(vup, _w));
    _v = cross(_w, _u);

    Vec3 viewportU = viewportWidth * _u;
    Vec3 viewportV = viewportHeight * -_v;

    _pixelDeltaU = viewportU / imageWidth;
    _pixelDeltaV = viewportV / _imagHeight;

    auto viewportUpperLeft = _center - (focusDistance * _w) - viewportU / 2 - viewportV / 2;
    _pixel00Loc = viewportUpperLeft + 0.5 * (_pixelDeltaU + _pixelDeltaV);

    auto defocusRadius = focusDistance * std::tan(degreeToRadians(defocusAngle / 2));
    _defocusDiskU = _u * defocusRadius;
    _defocusDiskV = _v * defocusRadius;
}
Ray Camera::_getRay(std::size_t i, std::size_t j) const 
{
    auto offset = _sampleSquare();
    auto pixelSample = _pixel00Loc
        + ((i + offset.x()) * _pixelDeltaU)
        + ((j + offset.y()) * _pixelDeltaV);

    auto rayOrig = (defocusAngle <= 0) ? _center : defocusDiskSample();
    auto rayDir = pixelSample - rayOrig;
    auto rayTime = randomDouble();

    return {rayOrig, rayDir, rayTime};
}

Vec3 Camera::_sampleSquare() const 
{
    return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}
Point3 Camera::defocusDiskSample() const 
{
    auto p = randomInUnitDisk();
    return _center + (p[0] * _defocusDiskU) + (p[1] * _defocusDiskV);
}
