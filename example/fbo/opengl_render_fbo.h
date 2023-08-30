#pragma once

#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderFrameBuffer : public OpenGLRender {
public:
    OpenGLRenderFrameBuffer();
    ~OpenGLRenderFrameBuffer();
    bool Init() override;
    void Destroy() override;
    std::string GetVertexShader() override;
    std::string GetFragmentShader() override;
    bool DrawGraphics(uint32_t width, uint32_t height) override;

private:
    uint32_t LoadTexture(const std::string& texture_path);
    std::vector<float> GetObjectVertices();
    std::vector<float> GetFloorVertices();
    std::vector<float> GetFBOVertices();

private:
    std::vector<float> object_vertices_{};
    std::vector<float> floor_vertices_{};
    std::vector<float> fbo_vertices_{};

    uint32_t fbo_vbo_;
    uint32_t fbo_object_vbo_;
    uint32_t fbo_floor_vbo_;
    uint32_t fbo_object_texture_;
    uint32_t fbo_floor_texture_;
    uint32_t fbo_texture_;

    uint32_t fbo_;
    uint32_t rbo_;

    Shader fbo_shader_{};
    Shader shader_;
};