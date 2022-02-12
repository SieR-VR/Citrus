#include "material.h"

Material::Material(MaterialType type, const Vec& color, const Vec& emission) : type(type), color(color), emission(emission) {}