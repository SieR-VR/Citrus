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
    shader = new Shader("shader", "/home/sier/Citrus/shaders/shader.vert", "/home/sier/Citrus/shaders/shader.frag");
    camera->registerShader(shader);
    scene->registerShader(shader);

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

        if (event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.sym == SDLK_w)
                camera->position.z -= 10;
            if (event->key.keysym.sym == SDLK_s)
                camera->position.z += 10;

            if (event->key.keysym.sym == SDLK_a)
                camera->position.x -= 10;
            if (event->key.keysym.sym == SDLK_d)
                camera->position.x += 10;

            if (event->key.keysym.sym == SDLK_q)
                camera->position.y -= 10;
            if (event->key.keysym.sym == SDLK_e)
                camera->position.y += 10;
        }
    }
}