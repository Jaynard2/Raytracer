#include <cstdlib>
#include <filesystem>

#include <fmt/format.h>
#include <fmt/std.h>
#include <memory>

#include "BVHNode.hpp"
#include "Camera.hpp"
#include "Hittable.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"
#include "rtweekend.hpp"

int main()
{
    HittableList world;

    auto groundMat = std::make_shared<Lambertian>(Color{0.5, 0.5, 0.5});
    world.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, groundMat));

    auto glassMat = std::make_shared<Dielectric>(1.5);

    for(int a = -11; a < 11; ++a)
    {
        for(int b = -11; b < 11; ++b)
        {
            auto choooseMat = randomDouble();
            Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> mat;
                if(choooseMat < 0.8)
                {
                    // Diffuse
                    auto aldebo = Color::random() * Color::random();
                    mat = std::make_shared<Lambertian>(aldebo);
                    world.add(std::make_shared<Sphere>(center, 0.2, mat));
                }
                else if(choooseMat < 0.95)
                {
                    // Metal
                    auto aldebo = Color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    mat = std::make_shared<Metal>(aldebo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, mat));
                }
                else
                {
                    // Glass
                    world.add(std::make_shared<Sphere>(center, 0.2, glassMat));
                }
            }
        }
    }

    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, glassMat));

    auto mat1 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, mat1));

    auto mat2 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, mat2));

    world = HittableList(std::make_shared<BVHNode>(world));

    Camera cam("image.ppm");
    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.vfov = 20;
    cam.lookFrom = {13, 2, 3};
    cam.lookat = {0, 0, 0};
    cam.vup = {0, 1, 0};
    cam.defocusAngle = 0.6;
    cam.focusDistance = 10.0;

    cam.render(world);

    return 0;
}
