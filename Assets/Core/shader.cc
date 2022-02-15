#include <GL/glew.h>
#include <SDL_image.h>
#include <fstream>
#include <iostream>

#include "shader.h"
#include "vec.h"

Shader::Shader(const std::string &name, const std::string &vertex_shader_path, const std::string &fragment_shader_path)
    : name(name)
{
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if (vertex_shader == 0 || fragment_shader == 0) {
        throw std::runtime_error("Shader::Shader: Failed to create shader");
    }

    std::ifstream vertex_shader_file(vertex_shader_path);
    std::ifstream fragment_shader_file(fragment_shader_path);

    if (!vertex_shader_file.is_open() || !fragment_shader_file.is_open()) {
        throw std::runtime_error("Shader::Shader: Failed to open shader file");
    }

    std::string vertex_shader_string((std::istreambuf_iterator<char>(vertex_shader_file)), std::istreambuf_iterator<char>());
    std::string fragment_shader_string((std::istreambuf_iterator<char>(fragment_shader_file)), std::istreambuf_iterator<char>());

    if (vertex_shader_string.empty() || fragment_shader_string.empty()) {
        throw std::runtime_error("Shader::Shader: Failed to read shader file");
    }

    const char *vertex_shader_cstr = vertex_shader_string.c_str();
    const char *fragment_shader_cstr = fragment_shader_string.c_str();

    glShaderSource(vertex_shader, 1, &vertex_shader_cstr, NULL);
    glShaderSource(fragment_shader, 1, &fragment_shader_cstr, NULL);

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    GLint vertex_shader_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_status);
    if (vertex_shader_status != GL_TRUE) {
        GLint vertex_shader_log_length;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &vertex_shader_log_length);

        char *vertex_shader_log = new char[vertex_shader_log_length];
        glGetShaderInfoLog(vertex_shader, vertex_shader_log_length, NULL, &vertex_shader_log[0]);

        throw std::runtime_error("Shader::Shader: Failed to compile vertex shader: " + std::string(&vertex_shader_log[0]));
    }

    GLint fragment_shader_status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_status);
    if (fragment_shader_status != GL_TRUE) {
        GLint fragment_shader_log_length;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &fragment_shader_log_length);

        char *fragment_shader_log = new char[fragment_shader_log_length];
        glGetShaderInfoLog(fragment_shader, fragment_shader_log_length, NULL, &fragment_shader_log[0]);

        throw std::runtime_error("Shader::Shader: Failed to compile fragment shader: " + std::string(&fragment_shader_log[0]));
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success = true;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        char *info_log = new char[info_log_length];
        glGetProgramInfoLog(program, info_log_length, NULL, info_log);
        throw std::runtime_error("Shader::Shader: Failed to link shader: " + std::string(info_log));
    }
}

Shader::~Shader()
{
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);
}

void Shader::addVariable(const std::string &name)
{
    variables[name] = glGetUniformLocation(program, name.c_str());

    if (variables[name] == GL_INVALID_INDEX) {
        throw std::runtime_error("Shader::addVariable: Failed to get uniform location for '" + name + "'");
    }
}

uint32_t Shader::getVariable(const std::string &name)
{
    return variables[name];
}

void Shader::activate()
{
    glUseProgram(program);
}

void Shader::deactivate()
{
    glUseProgram(0);
}

template<>
void Shader::setVariable<float>(const std::string &name, const float& value)
{
    glUniform1f(variables[name], value);
}

template<>
void Shader::setVariable<Vec2>(const std::string &name, const Vec2& value)
{
    glUniform2f(variables[name], value.x, value.y);
}

template<>
void Shader::setVariable<Vec3>(const std::string &name, const Vec3& value)
{
    glUniform3f(variables[name], value.x, value.y, value.z);
}

template<>
void Shader::setVariable<Vec4>(const std::string &name, const Vec4& value)
{
    glUniform4f(variables[name], value.x, value.y, value.z, value.w);
}

template<>
void Shader::setVariable<int>(const std::string &name, const int& value)
{
    glUniform1i(variables[name], value);
}