#pragma once

#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderMultiPrimitive : public OpenGLRender {
public:
    OpenGLRenderMultiPrimitive();
    ~OpenGLRenderMultiPrimitive();
    bool Init() override;
    void Destroy() override;
    std::string GetVertexShader() override;
    std::string GetFragmentShader() override;
    bool DrawGraphics(uint32_t width, uint32_t height) override;

private:
    Shader shader_;
    uint32_t vbo_;
};