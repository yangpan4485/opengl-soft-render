#include "opengl_render_texture_advanced.h"

#include <iostream>
#include "stb_image.h"

#include "vertex_compiler_texture_advanced.h"
#include "pixel_compiler_texture.h"
#include "compiler_manager.h"

namespace {
std::vector<float> vertices{-5.0f, -0.5f,  5.0f, 0.0f, 1.0f,
                            -5.0f, -0.5f, -5.0f, 0.0f, 0.0f,
                             5.0f, -0.5f,  5.0f, 1.0f, 1.0f,
                             5.0f, -0.5f, -5.0f, 1.0f, 0.0f};
}

OpenGLRenderTextureAdvanced::OpenGLRenderTextureAdvanced() = default;

OpenGLRenderTextureAdvanced::~OpenGLRenderTextureAdvanced() = default;

bool OpenGLRenderTextureAdvanced::Init() {
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerTextureAdvanced());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTexture());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::string texture_path = process_path_ + "/resource/metal.png";
    texture_ = LoadTexture(texture_path.c_str());
    if(!glIsTexture(texture_)) {
        return false;
    }

    return true;
}

void OpenGLRenderTextureAdvanced::Destroy() {
    glDeleteBuffers(1, &vbo_);
    glDeleteTextures(1, &texture_);
}

std::string OpenGLRenderTextureAdvanced::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_texture_advanced.vs";
    return path;
}

std::string OpenGLRenderTextureAdvanced::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader_texture.fs";
    return path;

}

bool OpenGLRenderTextureAdvanced::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)window_width_ / (float)window_height_, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 model = glm::mat4(1.0f);
    shader_.SetUniform("view", view);
    shader_.SetUniform("projection", projection);
    shader_.SetUniform("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    return true;
}

uint32_t OpenGLRenderTextureAdvanced::LoadTexture(const std::string& texture_path) {
    uint32_t texture;
    int width, height, channels_;
    uint8_t* data = stbi_load(texture_path.c_str(), &width, &height, &channels_, 0);
    if(!data) {
        std::cout << "stbi load failed" << std::endl;
        stbi_image_free(data);
        return texture;
    }
    GLenum format;
    if (channels_ == 3) {
        format = GL_RGB;
    } else if (channels_ == 4) {
        format = GL_RGBA;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}