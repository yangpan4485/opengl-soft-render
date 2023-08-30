#include "opengl_render_blend_test.h"

#include <iostream>
#include "stb_image.h"

#include "vertex_compiler_texture_advanced.h"
#include "pixel_compiler_texture.h"
#include "compiler_manager.h"

namespace {
std::vector<glm::vec3> vegetation {
    glm::vec3(-3.0f, 0.0f, -0.48f),
    glm::vec3( 1.5f, 0.0f, 0.51f),
    glm::vec3( 0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3(-1.5f, 0.0f, -0.6f)
};
}

OpenGLRenderBlendTest::OpenGLRenderBlendTest() {
}

OpenGLRenderBlendTest::~OpenGLRenderBlendTest() {
}

bool OpenGLRenderBlendTest::Init() {
    glEnable(GL_DEPTH_TEST);
    mask_ = mask_ | GL_DEPTH_BUFFER_BIT;
    glEnable(GL_BLEND);
    glBlendColor(0, 0, 0, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerTextureAdvanced());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTexture());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);
    
    object_vertices_ = GetObjectVertices();
    floor_vertices_ = GetFloorVertices();
    transparent_vertices_ = GetTransparentVertices();
    glGenBuffers(1, &blend_object_vbo_);
    glGenBuffers(1, &blend_floor_vbo_);
    glGenBuffers(1, &blend_transparent_vbo_);

    std::string blend_object_path = process_path_ + "/resource/marble.jpg";
    std::string blend_floor_path = process_path_ + "/resource/metal.png";
    std::string blend_transparent_path = process_path_ + "/resource/window.png";

    blend_object_texture_ = LoadTexture(blend_object_path.c_str());
    blend_floor_texture_ = LoadTexture(blend_floor_path.c_str());
    blend_transparent_texture_ = LoadTexture(blend_transparent_path.c_str());
    
    return true;
}

void OpenGLRenderBlendTest::Destroy() {
    glDeleteBuffers(1, &blend_object_vbo_);
    glDeleteBuffers(1, &blend_floor_vbo_);
    glDeleteBuffers(1, &blend_transparent_vbo_);
    
    glDeleteTextures(1, &blend_object_texture_);
    glDeleteTextures(1, &blend_floor_texture_);
    glDeleteTextures(1, &blend_transparent_texture_);
}

std::string OpenGLRenderBlendTest::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_texture_advanced.vs";
    return path;
}

std::string OpenGLRenderBlendTest::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader_texture.fs";
    return path;
}

bool OpenGLRenderBlendTest::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)window_width_ / (float)window_height_, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    shader_.SetUniform("view", view);
    shader_.SetUniform("projection", projection);
    glm::mat4 model = glm::mat4(1.0f);

    glActiveTexture(GL_TEXTURE0);
#if 1
    glBindTexture(GL_TEXTURE_2D, blend_object_texture_);
    model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -1.0f));
    shader_.SetUniform("model", model);
    glBindBuffer(GL_ARRAY_BUFFER, blend_object_vbo_);
    glBufferData(GL_ARRAY_BUFFER, object_vertices_.size() * sizeof(float), &object_vertices_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    shader_.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, blend_floor_vbo_);
    glBufferData(GL_ARRAY_BUFFER, floor_vertices_.size() * sizeof(float), &floor_vertices_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindTexture(GL_TEXTURE_2D, blend_floor_texture_);
    model = glm::mat4(1.0f);
    shader_.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
#if 1
    glBindBuffer(GL_ARRAY_BUFFER, blend_transparent_vbo_);
    glBufferData(GL_ARRAY_BUFFER, transparent_vertices_.size() * sizeof(float), &transparent_vertices_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindTexture(GL_TEXTURE_2D, blend_transparent_texture_);
    glDepthMask(GL_FALSE);
    for (unsigned int i = 0; i < vegetation.size(); i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vegetation[i]);              
        shader_.SetUniform("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glDepthMask(GL_TRUE);
#endif
    return true;
}

uint32_t OpenGLRenderBlendTest::LoadTexture(const std::string& texture_path) {
    uint32_t texture;
    int width, height, channels_;
    uint8_t* data = stbi_load(texture_path.c_str(), &width, &height, &channels_, 0);
    if(!data) {
        std::cout << "stbi load failed" << std::endl;
        stbi_image_free(data);
        return texture;
    }
    // std::cout << "channels_: " << channels_ << std::endl;
    GLenum format;
    if (channels_ == 3) {
        format = GL_RGB;
    } else if (channels_ == 4) {
        format = GL_RGBA;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}

std::vector<float> OpenGLRenderBlendTest::GetFloorVertices() {
    std::vector<float> vertices{
             // positions          // texture Coords 
             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
             5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    return vertices;
}

std::vector<float> OpenGLRenderBlendTest::GetObjectVertices() {
    std::vector<float> vertices{
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
            // Left face
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
            // Right face
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
            // Top face
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
    };
    return vertices;
}

std::vector<float> OpenGLRenderBlendTest::GetTransparentVertices() {
    std::vector<float> vertices{
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    return vertices;
}