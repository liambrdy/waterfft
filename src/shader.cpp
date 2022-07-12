#include "shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <fstream>

static std::string TypeToString(u32 type) {
    switch (type) {
        case GL_VERTEX_SHADER: return "vertex";
        case GL_GEOMETRY_SHADER: return "geometry";
        case GL_TESS_CONTROL_SHADER: return "tess control";
        case GL_TESS_EVALUATION_SHADER: return "tess eval";
        case GL_FRAGMENT_SHADER: return "fragment";
        case GL_COMPUTE_SHADER: return "compute";
        default: assert(false && "Unknown shader type"); return "";
    }
}

static i32 LoadAndCreateShader(const std::string path, u32 type) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::printf("Failed to open file: %s\n", path.c_str());
        std::exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

    u32 shader = glCreateShader(type);
    const char *c_str = buffer.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    i32 status = 1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        i32 infoLogLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<u8> infoLog(infoLogLength);
        glGetShaderInfoLog(shader, infoLogLength, nullptr, (char *)infoLog.data());
        std::printf("Failed to compile %s shader:\n%s:%s\n", TypeToString(type).c_str(), path.c_str(), infoLog.data());
        std::exit(1);
    }

    return shader;
}

void ShaderCreate(Shader *shader, const ShaderCreateInfo *info) {
    auto &&indexToEnum = [](i32 i) -> u32 {
        switch (i) {
            case VertexShader: return GL_VERTEX_SHADER;
            case TessellationControlShader: return GL_TESS_CONTROL_SHADER;
            case TessellationEvalShader: return GL_TESS_EVALUATION_SHADER;
            case GeometryShader: return GL_GEOMETRY_SHADER;
            case FragmentShader: return GL_FRAGMENT_SHADER;
            case ComputeShader: return GL_COMPUTE_SHADER;
            default: assert(false && "Unknown index to shader type"); return 0;
        }
    };

    std::vector<u32> shaders;
    i32 shaderIndex = 0;
    for (int i = 0; i < ShaderTypeCount; i++) {
        std::bitset<4> bit = 1 << i;
        if (info->shaderBits.test(i)) {
            shaders.push_back(LoadAndCreateShader(info->shaders[shaderIndex++], indexToEnum(i)));
        }
    }

    auto &&checkProgram = [&](u32 program, u32 flag) {
        i32 result;
        glGetProgramiv(program, flag, &result);
        if (result != GL_TRUE) {
            i32 length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            std::vector<u8> infoLog(length);
            glGetProgramInfoLog(program, length, nullptr, (char *)infoLog.data());
            std::printf("Program error: %s\n", infoLog.data());

            for (auto &s : shaders) {
                glDetachShader(program, s);
                glDeleteShader(s);
            }

            std::exit(1);
        }
    };

    u32 program = glCreateProgram();
    for (u32 s : shaders)
        glAttachShader(program, s);

    glLinkProgram(program);
    checkProgram(program, GL_LINK_STATUS);
    glValidateProgram(program);
    checkProgram(program, GL_VALIDATE_STATUS);

    for (auto &s : shaders) {
        glDetachShader(program, s);
        glDeleteShader(s);
    }

    shader->id = program;
}

void ShaderBind(Shader *shader) {
    glUseProgram(shader->id);
}

static i32 GetUniformLocation(Shader *shader, const std::string &name) {
    return glGetUniformLocation(shader->id, name.c_str());
}

void ShaderSetInt(Shader *shader, const std::string &name, int x) {
    i32 loc = GetUniformLocation(shader, name);
    glUniform1i(loc, x);
}

void ShaderSetFloat(Shader *shader, const std::string &name, float x) {
    i32 loc = GetUniformLocation(shader, name);
    glUniform1f(loc, x);
}

void ShaderSetFloat2(Shader *shader, const std::string &name, float x, float y) {
    i32 loc = GetUniformLocation(shader, name);
    glUniform2f(loc, x, y);
}

void ShaderSetFloat3(Shader *shader, const std::string &name, float x, float y, float z) {
    i32 loc = GetUniformLocation(shader, name);
    glUniform3f(loc, x, y, z);
}

void ShaderSetFloat4(Shader *shader, const std::string &name, float x, float y, float z, float w) {
    i32 loc = GetUniformLocation(shader, name);
    glUniform4f(loc, x, y, z, w);
}

void ShaderSetFloat2(Shader *shader, const std::string &name, const glm::vec2 &val) {
    ShaderSetFloat2(shader, name, val.x, val.y);
}

void ShaderSetFloat3(Shader *shader, const std::string &name, const glm::vec3 &val) {
    ShaderSetFloat3(shader, name, val.x, val.y, val.z);
}

void ShaderSetFloat4(Shader *shader, const std::string &name, const glm::vec4 &val) {
    ShaderSetFloat4(shader, name, val.x, val.y, val.z, val.w);
}

void ShaderSetMat4(Shader *shader, const std::string &name, const glm::mat4 &mat) {
    i32 loc = GetUniformLocation(shader, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderBindUniformBlock(Shader *shader, const std::string &name, u32 binding) {
    u32 loc = glGetUniformBlockIndex(shader->id, name.c_str());
    glUniformBlockBinding(shader->id, loc, binding);
}

void ShaderDestroy(Shader *shader) {
    glUseProgram(0);
    glDeleteProgram(shader->id);
}
