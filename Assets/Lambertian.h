#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "Core/shader.h"
#include "Core/object.h"
#include "Core/ray.h"

class Lambertian : public Material {
public:
    Vec albedo;

    Lambertian(const Vec& albedo) : albedo(albedo) {}

    virtual void registerUniform(Shader *shader, const std::string &name);
    virtual void setUniform(Shader *shader, const std::string &name);

    virtual bool scatter(const Ray& ray, const HitRecord& record, Vec& attenuation, Ray& scattered) const;
};

#endif