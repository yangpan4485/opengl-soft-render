#include "shader.h"

#include <fstream>
#include <iostream>

#include "utils.h"

Shader::Shader() = default;

Shader::~Shader() {
    glReleaseShaderCompiler();
    glDetachShader(shader_program_, vertex_shader_);
    glDetachShader(shader_program_, fragment_shader_);
    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);
    glDeleteProgram(shader_program_);
}

void Shader::SetLocation(const std::string& key, int location) {
    location_map_[key] = location;
}

void Shader::SetUniform(const std::string& key, int v0) {
    glUniform1i(glGetUniformLocation(shader_program_, key.c_str()), v0);
}

void Shader::SetUniform(const std::string& key, int v0, int v1) {
    glUniform2i(glGetUniformLocation(shader_program_, key.c_str()), v0, v1);
}

void Shader::SetUniform(const std::string& key, int v0, int v1, int v2) {
    glUniform3i(glGetUniformLocation(shader_program_, key.c_str()), v0, v1, v2);
}

void Shader::SetUniform(const std::string& key, int v0, int v1, int v2, int v3) {
    glUniform4i(glGetUniformLocation(shader_program_, key.c_str()), v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string& key, float v0) {
    GLint location = glGetUniformLocation(shader_program_, key.c_str());
    glUniform1f(location, v0);
}

void Shader::SetUniform(const std::string& key, float v0, float v1) {
    glUniform2f(glGetUniformLocation(shader_program_, key.c_str()), v0, v1);
}

void Shader::SetUniform(const std::string& key, float v0, float v1, float v2) {
    glUniform3f(glGetUniformLocation(shader_program_, key.c_str()), v0, v1, v2);
}

void Shader::SetUniform(const std::string& key, float v0, float v1, float v2, float v3) {
    glUniform4f(glGetUniformLocation(shader_program_, key.c_str()), v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string& key, int count, int* value) {
}

void Shader::SetUniform(const std::string& key, glm::mat2 value) {
    int location = glGetUniformLocation(shader_program_, key.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& key, glm::mat3 value) {
    int location = glGetUniformLocation(shader_program_, key.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& key, glm::mat4 value) {
    int location = glGetUniformLocation(shader_program_, key.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& key, glm::vec1 value) {
    glUniform1fv(glGetUniformLocation(shader_program_, key.c_str()), 1, &value[0]);
}

void Shader::SetUniform(const std::string& key, glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(shader_program_, key.c_str()), 1, &value[0]);
}

void Shader::SetUniform(const std::string& key, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(shader_program_, key.c_str()), 1, &value[0]);
}

void Shader::SetUniform(const std::string& key, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(shader_program_, key.c_str()), 1, &value[0]);
}

void Shader::Use() {
    glUseProgram(shader_program_);
}

bool Shader::CreateShader(const std::string& vertex_path, const std::string& fragment_path) {
    GLint result;
    glGetIntegerv(GL_SHADER_COMPILER, &result);
    if(result != 1) {
        std::cout << "shader not apply source compiler" << std::endl;
        return false;
    }
    std::string vertex_shader_src = GetFileContent(vertex_path);
    std::string fragment_shader_src = GetFileContent(fragment_path);
    if(vertex_shader_src.empty() || fragment_shader_src.empty()) {
        return false;
    }
    
    vertex_shader_ = LoadShader(vertex_shader_src.c_str(), GL_VERTEX_SHADER);
    fragment_shader_ = LoadShader(fragment_shader_src.c_str(), GL_FRAGMENT_SHADER);
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader_);
    glAttachShader(shader_program_, fragment_shader_);
    for(const auto& val : location_map_) {
        glBindAttribLocation(shader_program_, val.second, val.first.c_str());
    }
    glLinkProgram(shader_program_);
    int  success;
    char infoLog[512];
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program_, 512, NULL, infoLog);
        std::cout << "infoLog: " << infoLog << std::endl;
        return false;
    }
    return true;
}

GLuint Shader::LoadShader(const char* shader_source, GLenum type) {
    GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1 , &shader_source , NULL);
	glCompileShader(shader);
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	return shader;
}

GLuint Shader::GetShaderPorgram() {
    return shader_program_;
}