#include "opengl_render_triangle_strip.h"

#include <iostream>

#include "pixel_compiler_triangle.h"
#include "vertex_compiler_triangle.h"
#include "compiler_manager.h"

namespace {
std::vector<float> vertices{-0.5f, -0.5f, 0.0f, 0.3, 0.5, 0.7,
                            -0.5f,  0.5f, 0.0f, 0.2, 0.8, 0.4,
                             0.5f, -0.5f, 0.0f, 0.1, 0.2, 0.3,
                             0.5f,  0.5f, 0.0f, 0.5, 0.7, 0.6,};
}

OpenGLRenderTriangleStrip::OpenGLRenderTriangleStrip() {
}

OpenGLRenderTriangleStrip::~OpenGLRenderTriangleStrip() {
}

bool OpenGLRenderTriangleStrip::Init() {
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerTriangle());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTriangle());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return true;
}

void OpenGLRenderTriangleStrip::Destroy() {
    glDeleteBuffers(1, &vbo_);
}

std::string OpenGLRenderTriangleStrip::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader.vs";
    return path;
}

std::string OpenGLRenderTriangleStrip::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader.fs";
    return path;
}

bool OpenGLRenderTriangleStrip::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    return true;
}