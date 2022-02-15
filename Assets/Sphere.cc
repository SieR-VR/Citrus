#include <cmath>
#include "Sphere.h"

#define EPSILON 0.001f

void Sphere::registerUniform(Shader *shader, const std::string &name)
{
	shader->addVariable(name + ".position");
	shader->addVariable(name + ".radius");
	
	material->registerUniform(shader, name + ".material");
}

void Sphere::setUniform(Shader *shader, const std::string &name)
{
	shader->setVariable(name + ".position", position);
	shader->setVariable(name + ".radius", radius);

	material->setUniform(shader, name + ".material");
}

bool Sphere::hit(const Ray &ray, float t_min, float t_max, HitRecord &record) const
{
	Vec oc = ray.origin - position;
	float a = ray.direction.dot(ray.direction);
	float b = oc.dot(ray.direction);
	float c = oc.dot(oc) - radius * radius;
	float discriminant = b * b - a * c;

	if (discriminant > 0.0f) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			record.t = temp;
			record.p = ray.pointAtParameter(record.t);
			record.normal = (record.p - position) / radius;
			record.material = material;

			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			record.t = temp;
			record.p = ray.pointAtParameter(record.t);
			record.normal = (record.p - position) / radius;
			record.material = material;

			return true;
		}
	}

	return false;
}

