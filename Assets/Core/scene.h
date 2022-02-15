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

    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record);
    bool traceRay(const Ray& ray, Color& color, Ray& scattered);
};

#endif