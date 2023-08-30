#include "opengl_render_texture_yuv.h"

#include <iostream>
#include "stb_image.h"
#include "utils.h"
#include "vertex_compiler_texture.h"
#include "pixel_compiler_texture_yuv.h"
#include "compiler_manager.h"

namespace {
std::vector<float> vertices{-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                            -1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
                             1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                             1.0f,  1.0f, 0.0f, 1.0f, 0.0f};
}

OpenGLRenderTextureYUV::OpenGLRenderTextureYUV() {
    y_data_ = new uint8_t[render_width_ * render_height_];
    u_data_ = new uint8_t[render_width_ * render_height_ / 4];
    v_data_ = new uint8_t[render_width_ * render_height_ / 4];

    std::string filename = process_path_ + "/resource/480x272_yuv420p.yuv";
    // std::cout << "filename: " << filename << std::endl;
    fin_.open(filename, std::ios::in | std::ios::binary);
    if(!fin_.is_open()) {
        std::cout << "open file failed" << std::endl;
    }
}

OpenGLRenderTextureYUV::~OpenGLRenderTextureYUV()  {
    if(y_data_) {
        delete[] y_data_;
        y_data_ = nullptr;
    }
    if(u_data_) {
        delete[] u_data_;
        u_data_ = nullptr;
    }
    if(v_data_) {
        delete[] v_data_;
        v_data_ = nullptr;
    }
    if(fin_.is_open()) {
        fin_.close();
    }
}

bool OpenGLRenderTextureYUV::Init() {
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerTexture());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTextureYUV());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glGenTextures(1, &texture_y_);
    glBindTexture(GL_TEXTURE_2D, texture_y_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    glGenTextures(1, &texture_u_);
    glBindTexture(GL_TEXTURE_2D, texture_u_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    glGenTextures(1, &texture_v_);
    glBindTexture(GL_TEXTURE_2D, texture_v_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    return true;
}

void OpenGLRenderTextureYUV::Destroy() {
    glDeleteBuffers(1, &vbo_);
    glDeleteTextures(1, &texture_y_);
    glDeleteTextures(1, &texture_u_);
    glDeleteTextures(1, &texture_v_);
}

std::string OpenGLRenderTextureYUV::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_texture.vs";
    return path;
}

std::string OpenGLRenderTextureYUV::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader_texture_yuv.fs";
    return path;

}

bool OpenGLRenderTextureYUV::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    fin_.read((char*)y_data_, render_width_ * render_height_);
    fin_.read((char*)u_data_, render_width_ * render_height_ / 4);
    fin_.read((char*)v_data_, render_width_ * render_height_ / 4);
    if(fin_.eof()) {
        fin_.clear();
        fin_.seekg(0, std::ios::beg);
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_y_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, render_width_, render_height_, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, y_data_);
    shader_.SetUniform("tex_y", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_u_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, render_width_ / 2, render_height_ / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, u_data_);
    shader_.SetUniform("tex_u", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_v_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, render_width_ / 2, render_height_ / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, v_data_);
    shader_.SetUniform("tex_v", 2);
    
    uint32_t size = vertices.size() / 5;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, size);
    return true;
}

uint32_t OpenGLRenderTextureYUV::LoadTexture(const std::string& texture_path) {
    return 0;
}