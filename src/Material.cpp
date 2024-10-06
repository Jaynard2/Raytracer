#include "Material.hpp"

#include "Hittable.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include "rtweekend.hpp"

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
    auto scatterDir = rec.normal + randomUnitVector();

    if(scatterDir.nearZero())
        scatterDir = rec.normal;

    scattered = Ray(rec.p, scatterDir, rayIn.time());
    attenuation = _albedo;
    return true;
}

bool Metal::scatter(const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    Vec3 reflected = reflect(rayIn.direction(), rec.normal);
    reflected = unitVector(reflected) + (_fuzz * randomUnitVector());
    scattered = Ray(rec.p, reflected, rayIn.time());
    attenuation = _albedo;
    return dot(scattered.direction(), rec.normal) > 0;
}
bool Dielectric::scatter(const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered) const 
{
    attenuation = Color(1.0, 1.0, 1.0);
    double ri = (rec.frontFace ? (1.0 / _refractIndex) : _refractIndex);
    Vec3 unitDir = unitVector(rayIn.direction());
    double cosTheta = std::min(dot(-unitDir, rec.normal), 1.0);
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0;
    Vec3 dir;
    if(cannotRefract || _reflectance(cosTheta, ri) > randomDouble())
        dir = reflect(unitDir, rec.normal);
    else
        dir = refract(unitDir, rec.normal, ri);

    scattered = Ray(rec.p, dir, rayIn.time());
    return true;
}
double Dielectric::_reflectance(double cos, double refractIndex) const 
{
    auto r0 = (1 - refractIndex) / (1 + refractIndex);
    r0 *= r0;
    return r0 + (1 - r0) * std::pow((1 - cos), 5);
}
