#include "opengl_render_line.h"

#include "pixel_compiler_triangle.h"
#include "vertex_compiler_line.h"
#include "compiler_manager.h"

namespace {
std::vector<float> vertices{-0.5f, -0.5f, 0.0f, 1.0f, 0.0, 0.0, 0.0,
                            -0.5f,  0.5f, 0.0f, 0.0f, 1.0, 1.0, 1.0,
                             0.5f, -0.5f, 0.0f, 1.0f, 0.3, 0.5, 0.7,
                             0.5f,  0.5f, 0.0f, 1.0f, 0.3, 0.5, 0.7,};
}

OpenGLRenderLine::OpenGLRenderLine() {

}

OpenGLRenderLine::~OpenGLRenderLine() {

}

bool OpenGLRenderLine::Init() {
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerLine());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTriangle());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return true;
}

void OpenGLRenderLine::Destroy() {
    glDeleteBuffers(1, &vbo_);
}

std::string OpenGLRenderLine::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader.vs";
    return path;
}

std::string OpenGLRenderLine::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader.fs";
    return path;
}

bool OpenGLRenderLine::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glDrawArrays(GL_LINES, 0, 4);
    return true;
}