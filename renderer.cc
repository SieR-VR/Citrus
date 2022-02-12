#include "renderer.h"

inline float clamp(float x) { return x < 0 ? 0 : x > 1 ? 1 : x; }
inline int toInt(float x) { return int(pow(clamp(x), 1 / 2.2) * 255 + .5); }

Renderer::Renderer(Scene *scene, Camera *camera) : scene(scene), camera(camera) {
    pixel_buffer = new Vec[camera->width * camera->height];
}

void Renderer::render(int samples) {
    int width = camera->width;
    int height = camera->height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vec color(0, 0, 0);
            for (int sy = 0; sy < 2; sy++) {
                for (int sx = 0; sx < 2; sx++) {
                    float u = (x + (float(sx) + rand() / (RAND_MAX / 2)) / 2) / width;
                    float v = (y + (float(sy) + rand() / (RAND_MAX / 2)) / 2) / height;
                    Ray ray = camera->getRay(u, v);
                    color += scene->traceRay(ray, 0);
                }
            }
            color /= 4;
            int ir = toInt(color.x);
            int ig = toInt(color.y);
            int ib = toInt(color.z);
            pixel_buffer[y * width + x] = Vec(ir, ig, ib);
        }
    }
}

void Renderer::save_image(const char *file_name) {
    int width = camera->width;
    int height = camera->height;
    FILE *f = fopen(file_name, "wb");

    uint64_t magic_number = 0x89504e470d0a1a0a;
    fwrite(&magic_number, sizeof(uint64_t), 1, f);

    

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vec color = pixel_buffer[y * width + x];
            int ir = toInt(color.x);
            int ig = toInt(color.y);
            int ib = toInt(color.z);
            fputc(ir, f);
            fputc(ig, f);
            fputc(ib, f);
        }
    }
    fclose(f);
}