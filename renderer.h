#ifndef RENDERER_H_
#define RENDERER_H_

#include "scene.h"
#include "camera.h"
#include "shader.h"

class Renderer
{
private:
    Scene *scene;
    Camera *camera;
    Shader *shader;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event *event;

    uint32_t render_VAO;
    uint32_t render_VBO;
    uint32_t render_EBO;

    bool firstMouse = true;
    int lastX;
    int lastY;
    int xpos;
    int ypos;

    float yaw = -90.0f;
    float pitch = 0.0f;

public:
    Renderer(Scene *scene, Camera *camera);
    void render();
    void update();
    void processInput();
};

#endif