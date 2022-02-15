#include <SDL.h>
#include <SDL_image.h>
#include "cpu_renderer.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

#pragma comment(lib, "SDL2_image")

inline float clamp(float x) { return x < 0 ? 0 : x > 1 ? 1
                                                       : x; }
inline int toInt(float x) { return int(clamp(x) * 255 + .5); }

CPURenderer::CPURenderer(Scene *scene, Camera *camera)
    : scene(scene), camera(camera)
{
}

void CPURenderer::render(int samples, int reflection_depth)
{
    int width = camera->width;
    int height = camera->height;

    uint8_t *pixels = new uint8_t[width * height * 3];
    std::vector<std::thread> threads;

    std::atomic_int finished_pixels(0);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            std::thread t([&, x, y]() {
                Vec color(0, 0, 0);

                for (int i = 0; i < samples; i++)
                {
                    Ray ray = camera->getRay(x + drand48(), y + drand48());
                    Ray scattered;

                    Color temp = Color(1, 1, 1);
                    for (int j = 0; j < reflection_depth; j++)
                    {
                        if (!scene->traceRay(ray, temp, scattered))
                            break;

                        ray = scattered;
                    }

                    color += temp;
                }

                color /= samples;

                pixels[(y * width + x) * 3 + 0] = toInt(color.x);
                pixels[(y * width + x) * 3 + 1] = toInt(color.y);
                pixels[(y * width + x) * 3 + 2] = toInt(color.z);

                finished_pixels++;
                if (finished_pixels % (width * height / 100) == 0)
                    std::cout << "Rendered " << finished_pixels << " pixels" << std::endl;
            });

            threads.push_back(std::move(t));
        }
    }

    for (auto &t : threads)
        t.join();

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(pixels, width, height,
                                                    24, width * 3,
                                                    0x000000FF,
                                                    0x0000FF00,
                                                    0x00FF0000,
                                                    0);
    SDL_SaveBMP(surface, "../../output.bmp");
}