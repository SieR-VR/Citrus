#include <cmath>
#include "camera.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

Camera::Camera(const Vec &position,
               const Vec &target,
               const int &width,
               const int &height,
               const float &fov,
               const float &vp_dist)
    : position(position),
      target(target),
      width(width),
      height(height),
      fov(fov),
      vp_dist(vp_dist)
{
    aspect = static_cast<float>(width) / static_cast<float>(height);
    direction = (target - position).normalize();

    Vec up = Vec(0.f, 1.f, 0.f);

    x_direction = direction.cross(up).normalize();
    y_direction = x_direction.cross(direction).normalize();
}

Ray Camera::getRay(int x, int y)
{
    Vec center = position + direction * vp_dist;
    double vp_width = 2 * vp_dist * std::tan((fov / 2) * M_PI / 180.0);
    double vp_height = 1. / aspect * vp_width;

    Vec start_pixel = center - x_direction * (vp_width / 2) + y_direction * (vp_height / 2);
    Vec target_pixel = start_pixel + x_direction * (vp_width * (x + 0.5) / width) - y_direction * (vp_height * (y + 0.5) / height);

    return Ray(position, (target_pixel - position).normalize());
}

void Camera::registerShader(Shader *shader)
{
    shader->addVariable("camera_position");
    // shader->addVariable("target");
    shader->addVariable("camera_direction");

    // shader->addVariable("width");
    // shader->addVariable("height");

    shader->addVariable("fov");
    shader->addVariable("aspect");
    shader->addVariable("vp_dist");

    shader->addVariable("x_direction");
    shader->addVariable("y_direction");
    // shader->addVariable("look_up");

    shader->setVariable("camera_position", position);
    // shader->setVariable("target", target);
    shader->setVariable("camera_direction", direction);

    // shader->setVariable("width", width);
    // shader->setVariable("height", height);

    shader->setVariable("fov", fov);
    shader->setVariable("aspect", aspect);
    shader->setVariable("vp_dist", vp_dist);

    shader->setVariable("x_direction", x_direction);
    shader->setVariable("y_direction", y_direction);
    // shader->setVariable("look_up", y_direction);
}