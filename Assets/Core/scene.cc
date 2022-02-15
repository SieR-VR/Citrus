#include "scene.h"

void Scene::addObject(std::string name, Object *object)
{
    objects[name] = object;
}

void Scene::registerUniform(Shader *shader)
{
    for (auto &object : objects)
        object.second->registerUniform(shader, object.first);
}

void Scene::setUniform(Shader *shader)
{
    for (auto &object : objects)
        object.second->setUniform(shader, object.first);
}

bool Scene::hit(const Ray &ray, float tMin, float tMax, HitRecord &record)
{
    bool hit = false;
    float closest = tMax;

    for (auto &object : objects)
    {
        HitRecord temp;
        if (object.second->hit(ray, tMin, closest, temp))
        {
            hit = true;
            closest = temp.t;
            record = temp;
        }
    }

    return hit;
}

bool Scene::traceRay(const Ray &ray, Color &color, Ray &scattered)
{
    HitRecord record;

    if (hit(ray, 0.001, FLT_MAX, record)) {
        Color attenuation;

        if (record.material->scatter(ray, record, attenuation, scattered)) {
            color = color * attenuation;
            return true;
        }
        else {
            color = Color(0, 0, 0);
            return false;
        }
    }
    else {
        Vec unitDirection = ray.direction.normalize();
        float t = 0.5 * (unitDirection.y + 1.0);
        color = color * (Color(1.0, 1.0, 1.0) * (1.0 - t)+ Color(0.5, 0.7, 1.0) * t);
        return false;
    }
}