#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

#include "Core/shader.h"
#include "Core/object.h"
#include "Core/ray.h"

class Dielectric : public Material {
public:
    float refIdx;

    Dielectric(float refIdx) : refIdx(refIdx) {}

    virtual void registerUniform(Shader *shader, const std::string &name);
    virtual void setUniform(Shader *shader, const std::string &name);

    virtual bool scatter(const Ray& ray, const HitRecord& record, Vec& attenuation, Ray& scattered) const;
};

#endif