#pragma once

#include "Hittable.hpp"
#include "Vec3.hpp"
#include <filesystem>
#include <fstream>

class Camera
{
public:
    Camera(std::filesystem::path outputFile);
    void render(const Hittable &world);

    double aspectRatio;
    std::size_t imageWidth;
    std::size_t samplesPerPixel;
    std::size_t maxDepth;

    double vfov;
    Point3 lookFrom;
    Point3 lookat;
    Vec3 vup;

    double defocusAngle;
    double focusDistance;

  private:
    int    _imagHeight;   // Rendered image height
    Point3 _center;         // Camera center
    Point3 _pixel00Loc;    // Location of pixel 0, 0
    Vec3   _pixelDeltaU;  // Offset to pixel to the right
    Vec3   _pixelDeltaV;  // Offset to pixel below
    double _pixelSamplesScale;
    Vec3 _v, _u, _w;
    Vec3 _defocusDiskU;
    Vec3 _defocusDiskV;
    
    std::fstream _output;

    void _init();
    Color _rayColor(const Ray& ray, std::size_t depth, const Hittable& world) const;
    Ray _getRay(std::size_t i, std::size_t j) const;
    Vec3 _sampleSquare() const;
    Point3 defocusDiskSample() const;
};
