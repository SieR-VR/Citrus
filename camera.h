#ifndef CAMERA_H_
#define CAMERA_H_

#include "vec.h"
#include "ray.h"
#include "shader.h"
class Camera
{
public:
    Vec position;
    Vec target;
    Vec direction;

    int width;
    int height;
    float fov;
    float aspect;
    float vp_dist;

    Vec x_direction;
    Vec y_direction;
    Vec look_up;

    Camera(
        const Vec& position,
        const Vec& target,
        const int& width,
        const int& height,
        const float& fov,
        const float& vp_dist);

    Ray getRay(int x, int y);

    void registerUniform(Shader* shader);
    void setUniform(Shader* shader);
};

#endif