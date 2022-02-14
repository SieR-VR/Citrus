#ifndef SCENE_H_
#define SCENE_H_

#include <map>
#include <string>

#include "object.h"
#include "shader.h"

class Scene
{
public:
    std::map<std::string, Object*> objects;

    void addObject(std::string name, Object* object);
    void registerUniform(Shader *shader);
    void setUniform(Shader *shader);

    ObjectIntersection intersect(const Ray& ray);
    Vec traceRay(const Ray& ray, int depth);
};

#endif