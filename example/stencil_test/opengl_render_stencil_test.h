#pragma once

#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderStencilTest : public OpenGLRender {
public:
    OpenGLRenderStencilTest();
    ~OpenGLRenderStencilTest();
    bool Init() override;
    void Destroy() override;
    std::string GetVertexShader() override;
    std::string GetFragmentShader() override;
    bool DrawGraphics(uint32_t width, uint32_t height) override;

private:
    uint32_t LoadTexture(const std::string& texture_path);
    std::vector<float> GetFloorVertices();
    std::vector<float> GetObjectVertices();

private:
    std::vector<float> object_vertices_{};
    std::vector<float> floor_vertices_{};
    uint32_t stencil_object_vbo_;
    uint32_t stencil_floor_vbo_;
    uint32_t stencil_object_texture_;
    uint32_t stencil_floor_texture_;
    Shader color_shader_{};
    Shader object_shader_{};
};