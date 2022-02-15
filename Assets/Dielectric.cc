#include "Dielectric.h"

void Dielectric::registerUniform(Shader *shader, const std::string &name)
{
    shader->addVariable(name + ".type");
    shader->addVariable(name + ".refIdx");
}

void Dielectric::setUniform(Shader *shader, const std::string &name)
{
    shader->setVariable(name + ".type", (int)MaterialType::DIELECTRIC);
    shader->setVariable(name + ".refIdx", refIdx);
}

bool Dielectric::scatter(const Ray& ray, const HitRecord& record, Vec& attenuation, Ray& scattered) const
{
    Vec outwardNormal;
    Vec reflected = reflect(ray.direction, record.normal);
    float niOverNt;
    attenuation = Vec(1.0, 1.0, 1.0);
    Vec refracted;
    float reflectProb;
    float cosine;

    if (ray.direction.dot(record.normal) > 0)
    {
        outwardNormal = record.normal * -1.0;
        niOverNt = refIdx;
        cosine = refIdx * ray.direction.dot(record.normal) / ray.direction.getLength();
    }
    else
    {
        outwardNormal = record.normal;
        niOverNt = 1.0 / refIdx;
        cosine = -ray.direction.dot(record.normal) / ray.direction.getLength();
    }

    if (refract(ray.direction, outwardNormal, niOverNt, refracted))
        reflectProb = schlick(cosine, refIdx);
    else
        reflectProb = 1.0;

    if (drand48() < reflectProb)
        scattered = Ray(record.p, reflected);
    else
        scattered = Ray(record.p, refracted);

    return true;
}