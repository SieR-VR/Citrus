#include "Metal.h"

void Metal::registerUniform(Shader *shader, const std::string &name)
{
    shader->addVariable(name + ".type");
    shader->addVariable(name + ".albedo");
    // shader->addVariable(name + ".fuzz");
}

void Metal::setUniform(Shader *shader, const std::string &name)
{
    shader->setVariable(name + ".type", (int)MaterialType::METAL);
    shader->setVariable(name + ".albedo", albedo);
    // shader->setVariable(name + ".fuzz", fuzz);
}

bool Metal::scatter(const Ray& ray, const HitRecord& record, Vec& attenuation, Ray& scattered) const
{
    Vec reflected = reflect(ray.direction.normalize(), record.normal);
    scattered = Ray(record.p, reflected + randomInUnitSphere() * fuzz);
    attenuation = albedo;
    return scattered.direction.dot(record.normal) > 0;
}
