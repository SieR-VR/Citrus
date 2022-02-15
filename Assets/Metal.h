#ifndef METAL_H_
#define METAL_H_

#include "Core/shader.h"
#include "Core/object.h"
#include "Core/ray.h"

class Metal : public Material {
public:
    Vec albedo;
    float fuzz;

    Metal(const Vec& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}

    virtual void registerUniform(Shader *shader, const std::string &name);
    virtual void setUniform(Shader *shader, const std::string &name);

    virtual bool scatter(const Ray& ray, const HitRecord& record, Vec& attenuation, Ray& scattered) const;
};

#endif