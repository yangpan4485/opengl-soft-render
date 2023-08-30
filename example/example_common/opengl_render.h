#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <GLES2/gl2.h>
#include <EGL/egl.h>

class OpenGLRender {
public:
    OpenGLRender();
    virtual ~OpenGLRender();

    virtual bool Init();
    virtual void Destroy();
    virtual std::string GetVertexShader();
    virtual std::string GetFragmentShader();
    virtual bool DrawGraphics(uint32_t width, uint32_t height);

    bool Draw(uint32_t width, uint32_t height);
    
protected:
    std::string process_path_{};
    bool init_{};
    uint32_t window_width_{};
    uint32_t window_height_{};
    GLbitfield mask_{GL_COLOR_BUFFER_BIT};
};