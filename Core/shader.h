#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <map>

class Shader
{
public:
    Shader(const std::string &name, const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    ~Shader();
    
    void addVariable(const std::string &name);
    uint32_t getVariable(const std::string &name);

    template<typename T>
    void setVariable(const std::string &name, const T &value);
    template<typename T>
    void setVariable(const std::string &name, const T *value, const unsigned long &count);

    std::string getName() const { return name; }

    void setShaderImg();
    void activate();
    void deactivate();

private:
    std::string name;
    uint32_t vertex_shader;
    uint32_t fragment_shader;
    uint32_t program;

    std::map<std::string, uint32_t> variables;
    std::map<std::string, uint32_t> img_variables;
};

#endif