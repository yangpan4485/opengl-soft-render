#pragma once

#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderBlendTest : public OpenGLRender {
public:
    OpenGLRenderBlendTest();
    ~OpenGLRenderBlendTest();
    bool Init() override;
    void Destroy() override;
    std::string GetVertexShader() override;
    std::string GetFragmentShader() override;
    bool DrawGraphics(uint32_t width, uint32_t height) override;

private:
    uint32_t LoadTexture(const std::string& texture_path);
    std::vector<float> GetFloorVertices();
    std::vector<float> GetObjectVertices();
    std::vector<float> GetTransparentVertices();

private:
    Shader shader_;
    std::vector<float> object_vertices_{};
    std::vector<float> floor_vertices_{};
    std::vector<float> transparent_vertices_{};
    uint32_t blend_object_vbo_;
    uint32_t blend_floor_vbo_;
    uint32_t blend_transparent_vbo_;
    uint32_t blend_object_texture_;
    uint32_t blend_floor_texture_;
    uint32_t blend_transparent_texture_;
};