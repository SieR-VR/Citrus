#ifndef CPU_RENDERER_H_
#define CPU_RENDERER_H_

#include "scene.h"
#include "camera.h"

class CPURenderer
{
private:
    Scene *scene;
    Camera *camera;    

public:
    CPURenderer(Scene *scene, Camera *camera);
    void render(int samples, int reflection_depth);
};

#endif