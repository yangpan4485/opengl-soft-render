#pragma once

#include <string>
#include <unordered_map>
#include <GLES2/gl2.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

class Shader {
public:
    Shader();
    ~Shader();

    void SetLocation(const std::string& key, int location);

    void SetUniform(const std::string& key, int v0);
    void SetUniform(const std::string& key, int v0, int v1);
    void SetUniform(const std::string& key, int v0, int v1, int v2);
    void SetUniform(const std::string& key, int v0, int v1, int v2, int v3);
    void SetUniform(const std::string& key, float v0);
    void SetUniform(const std::string& key, float v0, float v1);
    void SetUniform(const std::string& key, float v0, float v1, float v2);
    void SetUniform(const std::string& key, float v0, float v1, float v2, float v3);
    void SetUniform(const std::string& key, int count, int* value);
    void SetUniform(const std::string& key, glm::mat2 value);
    void SetUniform(const std::string& key, glm::mat3 value);
    void SetUniform(const std::string& key, glm::mat4 value);

    void SetUniform(const std::string& key, glm::vec1 value);
    void SetUniform(const std::string& key, glm::vec2 value);
    void SetUniform(const std::string& key, glm::vec3 value);
    void SetUniform(const std::string& key, glm::vec4 value);

    void Use();
    bool CreateShader(const std::string& vertex_path, const std::string& fragment_path);

    
    GLuint LoadShader(const char* shader_source, GLenum type);
    GLuint GetShaderPorgram();

private:

private:
    GLuint shader_program_{};
    GLuint vertex_shader_{};
    GLuint fragment_shader_{};
    std::unordered_map<std::string, int> location_map_{};
};