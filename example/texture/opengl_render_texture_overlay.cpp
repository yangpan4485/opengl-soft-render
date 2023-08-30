#include "opengl_render_texture_overlay.h"

#include <iostream>
#include "stb_image.h"

#include "vertex_compiler_texture.h"
#include "pixel_compiler_texture_overlay.h"
#include "compiler_manager.h"

namespace {
std::vector<float> vertices{-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                            -1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
                             1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                             1.0f,  1.0f, 0.0f, 1.0f, 0.0f};
}

OpenGLRenderTextureOverlay::OpenGLRenderTextureOverlay() = default;

OpenGLRenderTextureOverlay::~OpenGLRenderTextureOverlay() = default;

bool OpenGLRenderTextureOverlay::Init() {
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerTexture());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTextureOverlay());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::string texture_path = process_path_ + "/resource/container.jpg";
    texture_ = LoadTexture(texture_path.c_str());
    texture_path = process_path_ + "/resource/awesomeface.png";
    overlay_texture_ = LoadTexture(texture_path.c_str());
    shader_.SetUniform("vTexture", 0);
    shader_.SetUniform("vOverlayTexture", 1);

    return true;
}

void OpenGLRenderTextureOverlay::Destroy() {
    glDeleteBuffers(1, &vbo_);
    glDeleteTextures(1, &overlay_texture_);
    glDeleteTextures(1, &texture_);
}

std::string OpenGLRenderTextureOverlay::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_texture.vs";
    return path;
}

std::string OpenGLRenderTextureOverlay::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader_texture_overlay.fs";
    return path;

}

bool OpenGLRenderTextureOverlay::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, overlay_texture_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    return true;
}

uint32_t OpenGLRenderTextureOverlay::LoadTexture(const std::string& texture_path) {
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}