#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include "vec.h"
#include "ray.h"
#include "shader.h"

enum class MaterialType : int {
    DIFFUSE,
    SPECULAR,
    EMISSION,
};

class Material {
public:
    MaterialType type;
    Vec color;
    Vec emission;
    Material(MaterialType type = MaterialType::EMISSION, const Vec& color = Vec(1, 1, 1), const Vec& emission = Vec(0, 0, 0));
    void registerUniform(Shader *shader, const std::string &name);
    void setUniform(Shader *shader, const std::string &name);
};

#endif