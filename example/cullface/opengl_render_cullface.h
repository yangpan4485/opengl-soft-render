#pragma once

#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderCullface : public OpenGLRender {
public:
    OpenGLRenderCullface();
    ~OpenGLRenderCullface();
    bool Init() override;
    void Destroy() override;
    std::string GetVertexShader() override;
    std::string GetFragmentShader() override;
    bool DrawGraphics(uint32_t width, uint32_t height) override;

private:
    uint32_t LoadTexture(const std::string& texture_path);

private:
    Shader shader_;
    uint32_t vbo_;
    uint32_t texture_;
    uint32_t last_window_width_{};
    uint32_t last_window_height_{};
};