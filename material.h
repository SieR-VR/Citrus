#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vec.h"
#include "ray.h"

enum class MaterialType {
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
};

#endif