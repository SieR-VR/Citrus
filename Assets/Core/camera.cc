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

Ray Camera::getRay(const int &x, const int &y) const
{
    Vec center = position + direction * vp_dist;
    float vp_width = 2.f * tan(fov / 2.f) * vp_dist;
    float vp_height = vp_width / 1. / aspect;

    Vec start_pixel = center - x_direction * vp_width / 2.f +
                          y_direction * vp_height / 2.f;
    Vec target_pixel = start_pixel + x_direction * x /
                                          static_cast<float>(width) * vp_width -
                                      y_direction * y /
                                          static_cast<float>(height) * vp_height; 

    return Ray(position, (target_pixel - position).normalize());
}

void Camera::updatePosition(const Vec &position) {
    this->position = position;
}

void Camera::updateDirection(const Vec &direction) {
    this->direction = direction;
    Vec up = Vec(0.f, 1.f, 0.f);

    x_direction = direction.cross(up).normalize();
    y_direction = x_direction.cross(direction).normalize();
}

void Camera::registerUniform(Shader *shader)
{
    shader->addVariable("camera.position");
    shader->addVariable("camera.direction");

    shader->addVariable("camera.width");
    shader->addVariable("camera.height");

    shader->addVariable("camera.fov");
    shader->addVariable("camera.aspect");
    shader->addVariable("camera.vp_dist");

    shader->addVariable("camera.x_direction");
    shader->addVariable("camera.y_direction");

    shader->addVariable("camera.samples");
}

void Camera::setUniform(Shader *shader)
{
    shader->setVariable("camera.position", position);
    shader->setVariable("camera.direction", direction);

    shader->setVariable("camera.width", width);
    shader->setVariable("camera.height", height);

    shader->setVariable("camera.fov", fov);
    shader->setVariable("camera.aspect", aspect);
    shader->setVariable("camera.vp_dist", vp_dist);

    shader->setVariable("camera.x_direction", x_direction);
    shader->setVariable("camera.y_direction", y_direction);

    shader->setVariable("camera.samples", 16);
}