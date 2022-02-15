#ifndef OBJECT_H_
#define OBJECT_H_

#include "vec.h"
#include "ray.h"
#include "shader.h"
#include "material.h"

class Object {
public:
    Vec position;

    Object(const Vec& position)
        : position(position) {}
    virtual void registerUniform(Shader *shader, const std::string &name) = 0;
    virtual void setUniform(Shader *shader, const std::string &name) = 0;

    virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& record) const = 0;
};

#endif