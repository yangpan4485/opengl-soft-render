#pragma once

#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderCubeMap : public OpenGLRender {
public:
    OpenGLRenderCubeMap();
    ~OpenGLRenderCubeMap();
    bool Init() override;
    void Destroy() override;
    std::string GetVertexShader() override;
    std::string GetFragmentShader() override;
    bool DrawGraphics(uint32_t width, uint32_t height) override;

private:
    uint32_t LoadTexture(const std::string& texture_path);
    uint32_t LoadCubeMap(const std::vector<std::string>& files);
    std::vector<float> GetCubeMapVertices();
    std::vector<float> GetObjectVertices();

private:
    uint32_t cube_map_texture_{};
    uint32_t cube_object_texture_{};
    uint32_t cube_object_vbo_;
    uint32_t cube_map_vbo_;
    Shader cube_map_shader_;
    Shader cube_object_shader_{};
    std::vector<float> cube_object_vertices_;
    std::vector<float> cube_map_vertices_;
    uint32_t last_window_width_{};
    uint32_t last_window_height_{};
};