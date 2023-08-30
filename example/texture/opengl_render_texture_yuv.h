#pragma once
#include <fstream>
#include "opengl_render.h"
#include "shader.h"

class OpenGLRenderTextureYUV : public OpenGLRender {
public:
    OpenGLRenderTextureYUV();
    ~OpenGLRenderTextureYUV();

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
    GLuint texture_y_;
    GLuint texture_u_;
    GLuint texture_v_;
    GLuint uniform_y_;
    GLuint uniform_u_;
    GLuint uniform_v_;

    uint8_t* y_data_{};
    uint8_t* u_data_{};
    uint8_t* v_data_{};
    uint32_t render_width_{480};
    uint32_t render_height_{272};
    std::ifstream fin_{};

};