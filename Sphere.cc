#include <cmath>
#include "Sphere.h"

#define EPSILON 0.001f

ObjectIntersection Sphere::getIntersection(const Ray &ray)
{
	bool hit = false;
	float distance = 0;
	Vec n = Vec();

	Vec op = position - ray.origin;
	float b = op.dot(ray.direction);
	float det = (b * b) - op.dot(op) + (radius * radius);

	if (det < 0)
		return ObjectIntersection(hit, distance, n, material);
	else
	{
		float t1 = b - std::sqrt(det);
		float t2 = b + std::sqrt(det);

		distance = t1 > EPSILON ? t1 : (t2 > EPSILON ? t2 : 0);
		if (distance != 0)
		{
			hit = true;
			n = ((ray.origin + ray.direction * distance) - position).normalize();
		}
	}

	return ObjectIntersection(hit, distance, n, material);
}

void Sphere::registerUniform(Shader *shader, const std::string &name)
{
	shader->addVariable(name + ".position");
	shader->addVariable(name + ".radius");
	
	material.registerUniform(shader, name + ".material");
}

void Sphere::setUniform(Shader *shader, const std::string &name)
{
	shader->setVariable(name + ".position", position);
	shader->setVariable(name + ".radius", radius);

	material.setUniform(shader, name + ".material");
}