#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>

#include "vec.h"
#include "object.h"
#include "shader.h"

class Material;
struct HitRecord;

struct HitRecord {
    float t;
    Vec p;
    Vec normal;
    Material *material;
};

enum class MaterialType : int {
    LAMBERTIAN = 0,
    METAL = 1,
    DIELECTRIC = 2
};

class Material {
public:
    Material() {} 
    virtual void registerUniform(Shader *shader, const std::string &name) = 0;
    virtual void setUniform(Shader *shader, const std::string &name) = 0;
    virtual bool scatter(const Ray& ray, const HitRecord& record, Vec& attenuation, Ray& scattered) const = 0;
};

#endif