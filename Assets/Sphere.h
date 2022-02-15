#ifndef SPHERE_H_
#define SPHERE_H_

#include "Core/object.h"

class Sphere : public Object {
public:
    float radius;
    Material *material;

    Sphere(std::string name, const Vec& center, float radius, Material *material)
        : Object(center), radius(radius), material(material) {}

    virtual void registerUniform(Shader *shader, const std::string &name);
    virtual void setUniform(Shader *shader, const std::string &name);

    virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& record) const;
};

#endif