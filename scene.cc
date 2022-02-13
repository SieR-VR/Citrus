#include "scene.h"

void Scene::addObject(std::string name, Object* object)
{
    objects[name] = object;
}

ObjectIntersection Scene::intersect(const Ray& ray)
{
    ObjectIntersection closest;
    for (auto& object : objects)
    {
        ObjectIntersection intersection = object.second->getIntersection(ray);
        if (intersection.isHitted && (!closest.isHitted || intersection.distance < closest.distance))
        {
            closest = intersection;
        }
    }
    return closest;
}

Vec Scene::traceRay(const Ray &ray, int depth)
{
    ObjectIntersection intersection = intersect(ray);
    if (!intersection.isHitted)
        return Vec();

    return intersection.material.color;
}

void Scene::registerShader(Shader *shader)
{
    for (auto& object : objects)
    {
        shader->setVariable(object.first, (void *)object.second, object.second->getSizeInBytes());
    }
}