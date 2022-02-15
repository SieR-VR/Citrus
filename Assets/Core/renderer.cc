#include <cmath>
#include <stdexcept>
#include <GL/glew.h>
#include <SDL.h>

#include "renderer.h"

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

    window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, camera->width, camera->height, SDL_WINDOW_OPENGL);
    if (!window)
        throw std::runtime_error("SDL_CreateWindow: " + std::string(SDL_GetError()));

    context = SDL_GL_CreateContext(window);
    if (!context)
        throw std::runtime_error("SDL_GL_CreateContext: " + std::string(SDL_GetError()));

    glewInit();

    event = new SDL_Event();
    int res = SDL_SetRelativeMouseMode(SDL_TRUE);
    if (res != 0)
        throw std::runtime_error("SDL_SetRelativeMouseMode: " + std::string(SDL_GetError()));

    shader = new Shader("shader", "../../shaders/shader.vert", "../../shaders/shader.frag");
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

        if (event->type == SDL_MOUSEMOTION)
            processMouseMovement(event->motion.xrel, event->motion.yrel);
    }

    processKeyDown();
}

void Renderer::processKeyDown()
{
    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    const float speed = 0.1f;

    Vec position = camera->position;
    Vec currentDirection = camera->direction;

    if (state[SDL_SCANCODE_W])
        position += currentDirection * speed;
    if (state[SDL_SCANCODE_S])
        position -= currentDirection * speed;
    if (state[SDL_SCANCODE_A])
        position -= currentDirection.cross(Vec(0, 1, 0)).normalize() * speed;
    if (state[SDL_SCANCODE_D])
        position += currentDirection.cross(Vec(0, 1, 0)).normalize() * speed;
    if (state[SDL_SCANCODE_Q])
        position -= Vec(0, 1, 0) * speed;
    if (state[SDL_SCANCODE_E])
        position += Vec(0, 1, 0) * speed;

    camera->updatePosition(position);
}

void Renderer::processMouseMovement(int xrel, int yrel)
{
    float xoffset = xrel;
    float yoffset = yrel;

    xoffset *= 0.1f;
    yoffset *= -0.1f;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    Vec direction;
    direction.x = std::cos(yaw / 180.f * M_PI) * std::cos(pitch / 180.f * M_PI);
    direction.y = std::sin(pitch / 180.f * M_PI);
    direction.z = std::sin(yaw / 180.f * M_PI) * std::cos(pitch / 180.f * M_PI);
    
    camera->updateDirection(direction);
}