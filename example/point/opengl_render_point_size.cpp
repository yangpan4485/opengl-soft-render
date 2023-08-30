#include "opengl_render_point_size.h"

#include "pixel_compiler_triangle.h"
#include "vertex_compiler_point_size.h"
#include "compiler_manager.h"

namespace {
std::vector<float> vertices{-0.5f, -0.5f, 0.0f, 0.3, 0.5, 0.7,
                            -0.5f,  0.5f, 0.0f, 0.3, 0.5, 0.7,
                             0.5f, -0.5f, 0.0f, 0.3, 0.5, 0.7,
                             0.5f,  0.5f, 0.0f, 0.3, 0.5, 0.7,};
}

OpenGLRenderPointSize::OpenGLRenderPointSize() {
}

OpenGLRenderPointSize::~OpenGLRenderPointSize() {
}

bool OpenGLRenderPointSize::Init() {
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerPointSize());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTriangle());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    shader_.SetUniform("vPointSize", 10.0f);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return true;
}

void OpenGLRenderPointSize::Destroy() {
    glDeleteBuffers(1, &vbo_);
}

std::string OpenGLRenderPointSize::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_point_size.vs";
    return path;
}

std::string OpenGLRenderPointSize::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader.fs";
    return path;
}

bool OpenGLRenderPointSize::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glDrawArrays(GL_POINTS, 0, 4);
    return true;
}
