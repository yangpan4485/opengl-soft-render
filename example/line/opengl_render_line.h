#pragma once

#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderLine : public OpenGLRender {
public:
    OpenGLRenderLine();
    ~OpenGLRenderLine();
    bool Init() override;
    void Destroy() override;
    std::string GetVertexShader() override;
    std::string GetFragmentShader() override;
    bool DrawGraphics(uint32_t width, uint32_t height) override;

private:
    Shader shader_;
    uint32_t vbo_;
};