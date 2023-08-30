#include "opengl_render_texture_compress.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "vertex_compiler_texture.h"
#include "pixel_compiler_texture.h"
#include "compiler_manager.h"

namespace{
std::vector<float> vertices{-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                            -1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
                             1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                             1.0f,  1.0f, 0.0f, 1.0f, 0.0f};
}


OpenGLRenderTextureCompress::OpenGLRenderTextureCompress() {
}

OpenGLRenderTextureCompress::~OpenGLRenderTextureCompress() {
}

bool OpenGLRenderTextureCompress::Init() {
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerTexture());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTexture());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // testimage_RGBA8.pkm
    // container.pkm
    std::string texture_path = process_path_ + "/resource/container.pkm";
    texture_ = LoadTexture(texture_path.c_str());
    return true;
}

void OpenGLRenderTextureCompress::Destroy() {
    glDeleteBuffers(1, &vbo_);
    glDeleteTextures(1, &texture_);
}

std::string OpenGLRenderTextureCompress::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_texture.vs";
    return path;
}

std::string OpenGLRenderTextureCompress::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader_texture.fs";
    return path;
}

bool OpenGLRenderTextureCompress::DrawGraphics(uint32_t width, uint32_t height)  {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    return true;
}


uint32_t OpenGLRenderTextureCompress::LoadTexture(const std::string& texture_path) {
    uint32_t texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    std::ifstream fin(texture_path, std::ios::in | std::ios::binary);
    std::ostringstream tmp;
    tmp << fin.rdbuf();
    std::string data = tmp.str();
    
    int32_t width, height, format, size;
    if(data.substr(0, 3) == "PKM") {
        width = (int)data[12] * 256 + (int)data[13];
        height = (int)data[14] * 256 + (int)data[15];
        size = data.size() - 16;
        if (size * 2 == width * height) {
            format = GL_COMPRESSED_RGB8_ETC2;
        } else {
            format = GL_COMPRESSED_RGBA8_ETC2_EAC;
        }
    } else {
        return 0;
    }
    // std::cout << "width: " << width << std::endl;
    // std::cout << "height: " << height << std::endl;
    // std::cout << "size: " << size << std::endl;
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, size, data.c_str() + 16);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture_id;
}