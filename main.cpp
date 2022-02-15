#include <iostream>

#include "Assets/Core/cpu_renderer.h"
#include "Assets/Core/renderer.h"
#include "Assets/Sphere.h"

#include "Assets/Lambertian.h"
#include "Assets/Metal.h"
#include "Assets/Dielectric.h"

int main(int argc, char *argv[])
{
    int samples = 1;
    int replection_depth = 50;
    if (argc > 1)
        samples = std::stoi(argv[1]);
    if (argc > 2)
        replection_depth = std::stoi(argv[2]);

    Vec origin(0, 1.5, 4);
    Vec dest(0, 0, 0);

    Camera camera(origin, dest, 1280, 720, 90, 90);
    Scene scene;

    Lambertian floorMaterial(Vec(0.5, 0.5, 0.5));

    scene.addObject("floor_sphere", new Sphere("floor_sphere", Vec(0, -1000, -1), 1000, &floorMaterial));
    scene.addObject("sphere1", new Sphere("sphere1", Vec(0, 1, 0), 1.0, new Dielectric(1.5)));
    scene.addObject("sphere2", new Sphere("sphere2", Vec(-4, 1, 0), 1.0, new Lambertian(Vec(0.4, 0.2, 0.1))));
    scene.addObject("sphere3", new Sphere("sphere3", Vec(4, 1, 0), 1.0, new Metal(Vec(0.7, 0.6, 0.5), 0.0)));

    try
    {
        Renderer renderer(&scene, &camera);

        std::cout << "Rendering..." << std::endl;
        for (int frame = 0;; frame++) {
            renderer.render();
            renderer.update();

            if (frame % 60 == 0) {
                std::cout << "Frame: " << frame << std::endl;
                std::cout << "Position: " << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << std::endl;
            }
        }

        // renderer.render(samples, replection_depth);
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}