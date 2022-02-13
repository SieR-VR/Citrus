#include "material.h"

Material::Material(MaterialType type, const Vec& color, const Vec& emission) : type(type), color(color), emission(emission) {}

void Material::registerUniform(Shader *shader, const std::string &name) {
    shader->addVariable(name + ".type");
    shader->addVariable(name + ".color");
    shader->addVariable(name + ".emission");
}

void Material::setUniform(Shader *shader, const std::string &name) {
    shader->setVariable(name + ".type", (int)type);
    shader->setVariable(name + ".color", color);
    shader->setVariable(name + ".emission", emission);
}