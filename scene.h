#ifndef SCENE_H_
#define SCENE_H_

#include <map>
#include <string>

#include "object.h"

class Scene
{
public:
    std::map<std::string, Object*> objects;

    void addObject(std::string name, Object* object);
    ObjectIntersection intersect(const Ray& ray);
    Vec traceRay(const Ray& ray, int depth);
};

#endif