#include "Lambertian.h"

void Lambertian::registerUniform(Shader *shader, const std::string &name)
{
    shader->addVariable(name + ".type");
    shader->addVariable(name + ".albedo");
}

void Lambertian::setUniform(Shader *shader, const std::string &name)
{
    shader->setVariable(name + ".type", (int)MaterialType::LAMBERTIAN);
    shader->setVariable(name + ".albedo", albedo);
}

bool Lambertian::scatter(const Ray& ray, const HitRecord& record, Vec& attenuation, Ray& scattered) const
{
    Vec target = record.p + record.normal + randomInUnitSphere();
    scattered = Ray(record.p, (target - record.p).normalize());
    attenuation = albedo;
    return true;
}