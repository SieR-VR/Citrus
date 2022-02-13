#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "renderer.h"
#include "Sphere.h"

int main(int argc, char *argv[]) {
    Vec origin(0, 0, 250);
    Vec dest(0, 0, 0);

    Camera camera(origin, dest, 1280, 720, 500, 90);
    Scene scene;

    scene.addObject("sphere1", new Sphere("sphere1", Vec(0, 0, 0), 100, Material()));
    scene.addObject("sphere2", new Sphere("sphere2", Vec(0, -100.5, -1), 100, Material()));
    scene.addObject("sphere3", new Sphere("sphere3", Vec(300, 0, 0), 100, Material()));

    Renderer renderer(&scene, &camera);
    
    try {
        for (;;) {
            renderer.render();
            renderer.update();
            renderer.processInput();
        }
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
}