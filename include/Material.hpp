#pragma once

#include "Ray.hpp"
#include "Vec3.hpp"
class HitRecord;

class Material
{
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const { return false; }

private:

};

class Lambertian : public Material
{
public:
    Lambertian(const Color& albedo) : _albedo(albedo) {}
    virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

private:
    Color _albedo;

};

class Metal : public Material
{
public:
    Metal(const Color& albedo, double fuzz) : _albedo(albedo), _fuzz(fuzz) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

  private:
    Color _albedo;
    double _fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(double refractIndex) : _refractIndex(refractIndex) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

  private:
    double _refractIndex;

    double _reflectance(double cos, double refractIndex) const;
};
