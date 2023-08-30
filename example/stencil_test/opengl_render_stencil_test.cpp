#include "opengl_render_stencil_test.h"

#include <iostream>
#include "stb_image.h"

#include "vertex_compiler_texture_advanced.h"
#include "pixel_compiler_texture.h"
#include "vertex_compiler_color.h"
#include "pixel_compiler_color.h"
#include "compiler_manager.h"

OpenGLRenderStencilTest::OpenGLRenderStencilTest() {
}

OpenGLRenderStencilTest::~OpenGLRenderStencilTest() {
}

bool OpenGLRenderStencilTest::Init() {
    glEnable(GL_DEPTH_TEST);
    glClearStencil(0);
    // stencil test
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    mask_ = mask_ | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;

    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    object_shader_.CreateShader(vertex_shader_path, fragment_shader_path);

    vertex_shader_path = process_path_ + "/shader_source/color_vertex_shader.vs";
    fragment_shader_path = process_path_ + "/shader_source/color_fragment_shader.fs";
    color_shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    
    object_shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerTextureAdvanced());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerTexture());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    object_vertices_ = GetObjectVertices();
    floor_vertices_ = GetFloorVertices();
    
    glGenBuffers(1, &stencil_object_vbo_);
    glGenBuffers(1, &stencil_floor_vbo_);

    std::string stencil_object_path = process_path_ + "/resource/marble.jpg";
    std::string stencil_floor_path = process_path_ + "/resource/metal.png";

    stencil_object_texture_ = LoadTexture(stencil_object_path.c_str());
    stencil_floor_texture_ = LoadTexture(stencil_floor_path.c_str());
    object_shader_.SetUniform("vTexture", 0);
    return true;
}

void OpenGLRenderStencilTest::Destroy() {
    glDeleteBuffers(1, &stencil_object_vbo_);
    glDeleteBuffers(1, &stencil_floor_vbo_);
    glDeleteTextures(1, &stencil_object_texture_);
    glDeleteTextures(1, &stencil_floor_texture_);
}

std::string OpenGLRenderStencilTest::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_texture_advanced.vs";
    return path;
}

std::string OpenGLRenderStencilTest::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader_texture.fs";
    return path;
}

bool OpenGLRenderStencilTest::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)window_width_ / (float)window_height_, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), 
                                 glm::vec3(0.0f, 0.0f, 0.0f), 
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);
    color_shader_.Use();
    color_shader_.SetUniform("view", view);
    color_shader_.SetUniform("projection", projection);

    object_shader_.Use();
    object_shader_.SetUniform("view", view);
    object_shader_.SetUniform("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glStencilMask(0x00);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // glStencilFunc(GL_EQUAL, 0, 0xFF);
    glBindBuffer(GL_ARRAY_BUFFER, stencil_floor_vbo_);
    glBufferData(GL_ARRAY_BUFFER, floor_vertices_.size() * sizeof(float), &floor_vertices_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindTexture(GL_TEXTURE_2D, stencil_floor_texture_);
    model = glm::mat4(1.0f);
    object_shader_.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glBindTexture(GL_TEXTURE_2D, stencil_object_texture_);
    model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -1.0f));
    object_shader_.SetUniform("model", model);
    glBindBuffer(GL_ARRAY_BUFFER, stencil_object_vbo_);
    glBufferData(GL_ARRAY_BUFFER, object_vertices_.size() * sizeof(float), &object_vertices_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    object_shader_.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glStencilMask(0x00);
    // 防止地板覆盖了图形
    glDisable(GL_DEPTH_TEST);
    color_shader_.Use();
    std::shared_ptr<VertexCompiler> vertex_compiler(new VertexCompilerColor());
    std::shared_ptr<PixelCompiler> pixel_compiler(new PixelCompilerColor());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);

    float scale = 1.1f;
    // cubes
    glBindBuffer(GL_ARRAY_BUFFER, stencil_object_vbo_);
    glBufferData(GL_ARRAY_BUFFER, object_vertices_.size() * sizeof(float), &object_vertices_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    color_shader_.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    color_shader_.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
    return true;
}

uint32_t OpenGLRenderStencilTest::LoadTexture(const std::string& texture_path) {
    uint32_t texture;
    int width, height, channels_;
    uint8_t* data = stbi_load(texture_path.c_str(), &width, &height, &channels_, 0);
    if(!data) {
        std::cout << "stbi load failed" << std::endl;
        stbi_image_free(data);
        return texture;
    }
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

std::vector<float> OpenGLRenderStencilTest::GetFloorVertices() {
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

std::vector<float> OpenGLRenderStencilTest::GetObjectVertices() {
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