#ifndef SPHERE_H_
#define SPHERE_H_

#include "object.h"

class Sphere : public Object {
public:
    float radius;
    Material material;

    Sphere(std::string name, const Vec& center, float radius, const Material& material)
        : Object(center), radius(radius), material(material) {}

    virtual ObjectIntersection getIntersection(const Ray& ray);
    virtual void registerUniform(Shader *shader, const std::string &name);
    virtual void setUniform(Shader *shader, const std::string &name);
};

#endif