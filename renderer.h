#ifndef RENDERER_H_
#define RENDERER_H_

#include "scene.h"
#include "camera.h"

class Renderer 
{
private:
    Scene *scene;
    Camera *camera;
    Vec *pixel_buffer;

public:
    Renderer(Scene *scene, Camera *camera);
    void render(int samples);
    void save_image(const char *filename);
};

#endif