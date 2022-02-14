#ifndef OBJECT_H_
#define OBJECT_H_

#include "vec.h"
#include "shader.h"
#include "material.h"

struct ObjectIntersection {
    bool isHitted;
    float distance;
    Vec normal;
    Material material;

    ObjectIntersection(bool isHitted = false, float distance = 0, const Vec& normal = Vec(), const Material& material = Material())
        : isHitted(isHitted), distance(distance), normal(normal), material(material) {}
};

class Object {
public:
    Vec position;

    Object(const Vec& position)
        : position(position) {}
    virtual ObjectIntersection getIntersection(const Ray& ray) = 0;
    virtual void registerUniform(Shader *shader, const std::string &name) = 0;
    virtual void setUniform(Shader *shader, const std::string &name) = 0;
};

#endif