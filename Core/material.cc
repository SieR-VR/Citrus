#include "material.h"

Material::Material(MaterialType type, const Vec& color, const Vec& emission) : type(type), color(color), emission(emission) {}

void Material::registerUniform(Shader *shader, const std::string &name) {
    shader->addVariable(name + ".color");
}

void Material::setUniform(Shader *shader, const std::string &name) {
    shader->setVariable(name + ".color", color);
}