#include <iostream>

#include "renderer.h"
#include "Sphere.h"

int main(int argc, char *argv[]) {
    int samples = 4;
    if (argc > 1)
        samples = atoi(argv[1]);

    Vec origin(0, 0, 500);
    Vec dest(0, 0, 0);

    Camera camera(origin, dest, 100, 80, 500, 90);
    Scene scene;

    scene.addObject("sphere", new Sphere(Vec(0, 0, -1), 100, Material()));
    scene.addObject("sphere", new Sphere(Vec(0, -100.5, -1), 100, Material()));

    Renderer renderer(&scene, &camera);
    renderer.render(samples);
    renderer.save_image("image.bmp");
}