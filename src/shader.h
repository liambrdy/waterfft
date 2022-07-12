#ifndef SHADER_H_
#define SHADER_H_

#include "types.h"

#include <string>
#include <vector>
#include <array>
#include <bitset>

#include <glm/glm.hpp>

enum ShaderType {
VertexShader,
TessellationControlShader,
TessellationEvalShader,
GeometryShader,
FragmentShader,
ComputeShader,
ShaderTypeCount
};

struct ShaderCreateInfo {
    std::array<std::string, ShaderTypeCount> shaders;
    std::bitset<ShaderTypeCount> shaderBits;
};

struct Shader {
    u32 id;
};

void ShaderCreate(Shader *shader, const ShaderCreateInfo *info);
void ShaderBind(Shader *shader);

void ShaderSetInt(Shader *shader, const std::string &name, int x);

void ShaderSetFloat(Shader *shader, const std::string &name, float x);
void ShaderSetFloat2(Shader *shader, const std::string &name, float x, float y);
void ShaderSetFloat3(Shader *shader, const std::string &name, float x, float y, float z);
void ShaderSetFloat4(Shader *shader, const std::string &name, float x, float y, float z, float w);

void ShaderSetFloat2(Shader *shader, const std::string &name, const glm::vec2 &val);
void ShaderSetFloat3(Shader *shader, const std::string &name, const glm::vec3 &val);
void ShaderSetFloat4(Shader *shader, const std::string &name, const glm::vec4 &val);

void ShaderSetMat4(Shader *shader, const std::string &name, const glm::mat4 &mat);

void ShaderBindUniformBlock(Shader *shader, const std::string &name, u32 binding);

void ShaderDestroy(Shader *shader);

#endif // SHADER_H_
