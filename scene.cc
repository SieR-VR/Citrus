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

void Scene::registerUniform(Shader *shader)
{
    for (auto& object : objects)
        object.second->registerUniform(shader, object.first);
}

void Scene::setUniform(Shader *shader)
{
    for (auto& object : objects)
        object.second->setUniform(shader, object.first);
}