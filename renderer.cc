#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "renderer.h"

inline float clamp(float x) { return x < 0 ? 0 : x > 1 ? 1
                                                       : x; }
inline int toInt(float x) { return int(clamp(x) * 255 + .5); }

float vertices[] = {
    -1, -1, 
    1, -1,
    1, 1,
    -1, 1};

uint32_t indices[] = {
    0, 1, 2,
    2, 3, 0};

Renderer::Renderer(Scene *scene, Camera *camera) : scene(scene), camera(camera)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, camera->width, camera->height, 0);
    if (!window)
        throw std::runtime_error("SDL_CreateWindow: " + std::string(SDL_GetError()));

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        throw std::runtime_error("SDL_CreateRenderer: " + std::string(SDL_GetError()));

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, camera->width, camera->height);

    glewInit();
    std::cout << glGetString(GL_VERSION) << std::endl;

    event = new SDL_Event();
    int res = SDL_SetRelativeMouseMode(SDL_TRUE);
    if (res != 0)
        throw std::runtime_error("SDL_SetRelativeMouseMode: " + std::string(SDL_GetError()));

    shader = new Shader("shader", "/home/sier/Citrus/shaders/shader.vert", "/home/sier/Citrus/shaders/shader.frag");
    camera->registerUniform(shader);
    scene->registerUniform(shader);

    glGenVertexArrays(1, &render_VAO);
    glGenBuffers(1, &render_VBO);
    glGenBuffers(1, &render_EBO);

    glBindVertexArray(render_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, render_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Renderer::render()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    shader->activate();
    camera->setUniform(shader);
    scene->setUniform(shader);

    glBindVertexArray(render_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    SDL_GL_SwapWindow(window);
}

void Renderer::update()
{
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
            exit(0);
    }
}

void Renderer::processInput()
{
    SDL_PumpEvents();
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    const float speed = 0.2;

    if (keyboard[SDL_SCANCODE_ESCAPE])
        exit(0);

    if (keyboard[SDL_SCANCODE_W])
        camera->position += Vec(0, 0, -speed);
    if (keyboard[SDL_SCANCODE_S])
        camera->position += Vec(0, 0, speed);
    if (keyboard[SDL_SCANCODE_A])
        camera->position += Vec(-speed, 0, 0);
    if (keyboard[SDL_SCANCODE_D])
        camera->position += Vec(speed, 0, 0);
    if (keyboard[SDL_SCANCODE_Q])
        camera->position += Vec(0, -speed, 0);
    if (keyboard[SDL_SCANCODE_E])
        camera->position += Vec(0, speed, 0);
    
    // SDL_GetMouseState(&xpos, &ypos);

    // if (firstMouse)
    // {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos;
    // lastX = xpos;
    // lastY = ypos;

    // xoffset *= speed;
    // yoffset *= speed;

    // yaw += xoffset;
    // pitch += yoffset;

    // if (pitch > 89.0f)
    //     pitch = 89.0f;
    // if (pitch < -89.0f)
    //     pitch = -89.0f;
    
    // Vec direction;
    // direction.x = cos(yaw) * cos(pitch);
    // direction.y = sin(pitch);
    // direction.z = sin(yaw) * cos(pitch);
    // camera->direction = direction.normalize();
}