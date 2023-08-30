#include "opengl_render_cube_map.h"

#include <iostream>
#include "stb_image.h"

#include "vertex_compiler_texture_advanced.h"
#include "pixel_compiler_texture.h"
#include "vertex_compiler_cube_map.h"
#include "pixel_compiler_cube_map.h"
#include "compiler_manager.h"

OpenGLRenderCubeMap::OpenGLRenderCubeMap() {
}

OpenGLRenderCubeMap::~OpenGLRenderCubeMap() {
}

bool OpenGLRenderCubeMap::Init() {
    mask_ = mask_ | GL_DEPTH_BUFFER_BIT;
    glEnable(GL_DEPTH_TEST);

    std::vector<std::string> file_path;
    file_path.push_back(process_path_ + "/resource/skybox/right.jpg");
    file_path.push_back(process_path_ + "/resource/skybox/left.jpg");
    file_path.push_back(process_path_ + "/resource/skybox/top.jpg");
    file_path.push_back(process_path_ + "/resource/skybox/bottom.jpg");
    file_path.push_back(process_path_ + "/resource/skybox/front.jpg");
    file_path.push_back(process_path_ + "/resource/skybox/back.jpg");
    cube_map_texture_ = LoadCubeMap(file_path);

    glGenBuffers(1, &cube_map_vbo_);
    glGenBuffers(1, &cube_object_vbo_);

    std::string texture_path = process_path_ + "/resource/container.jpg";
    cube_object_texture_ = LoadTexture(texture_path);
    
    std::string vertex_shader_path = GetVertexShader();
    std::string fragment_shader_path = GetFragmentShader();
    cube_object_shader_.CreateShader(vertex_shader_path, fragment_shader_path);

    cube_object_vertices_ = GetObjectVertices();
    cube_map_vertices_ = GetCubeMapVertices();

    glBindBuffer(GL_ARRAY_BUFFER, cube_object_vbo_);
    glBufferData(GL_ARRAY_BUFFER, cube_object_vertices_.size() * sizeof(float), &cube_object_vertices_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    cube_map_shader_.SetLocation("vCubePosition", 3);
    vertex_shader_path = process_path_ + "/shader_source/vertex_shader_cube_map.vs";
    fragment_shader_path = process_path_ + "/shader_source/fragment_shader_cube_map.fs";
    cube_map_shader_.CreateShader(vertex_shader_path, fragment_shader_path);
    glBindBuffer(GL_ARRAY_BUFFER, cube_map_vbo_);
    glBufferData(GL_ARRAY_BUFFER, cube_map_vertices_.size() * sizeof(float), &cube_map_vertices_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(3);

    cube_object_shader_.Use();
    cube_object_shader_.SetUniform("vTexture", 0);
    cube_map_shader_.Use();
    cube_map_shader_.SetUniform("skybox", 0);

    return true;
}

void OpenGLRenderCubeMap::Destroy() {
    glDeleteBuffers(1, &cube_map_vbo_);
    glDeleteBuffers(1, &cube_object_vbo_);
    glDeleteTextures(1, &cube_object_texture_);
    glDeleteTextures(1, &cube_map_texture_);
}

std::string OpenGLRenderCubeMap::GetVertexShader() {
    std::string path = process_path_ + "/shader_source/vertex_shader_texture_advanced.vs";
    return path;
}

std::string OpenGLRenderCubeMap::GetFragmentShader() {
    std::string path = process_path_ + "/shader_source/fragment_shader_texture.fs";
    return path;
}

bool OpenGLRenderCubeMap::DrawGraphics(uint32_t width, uint32_t height) {
    if(width != window_width_ || height != window_height_) {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, width, height);
    }
    std::shared_ptr<VertexCompiler> vertex_compiler{};
    std::shared_ptr<PixelCompiler> pixel_compiler{};
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(mask_);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), 
                                 glm::vec3(0.0f, 0.0f, 0.0f), 
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window_width_ / (float)window_height_, 0.1f, 100.0f);
    cube_object_shader_.Use();
    vertex_compiler.reset(new VertexCompilerTextureAdvanced());
    pixel_compiler.reset(new PixelCompilerTexture());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);
    model = glm::rotate(model, glm::radians((float)(45.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
    cube_object_shader_.SetUniform("model", model);
    cube_object_shader_.SetUniform("view", view);
    cube_object_shader_.SetUniform("projection", projection);

#if 1
    // cubes
    glBindBuffer(GL_ARRAY_BUFFER, cube_object_vbo_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_object_texture_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
#endif  
#if 1
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    cube_map_shader_.Use();
    vertex_compiler.reset(new VertexCompilerCubeMap());
    pixel_compiler.reset(new PixelCompilerCubeMap());
    CompilerManager::GetInstance().SetVertexCompiler(vertex_compiler);
    CompilerManager::GetInstance().SetPixelCompiler(pixel_compiler);
    view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
    cube_map_shader_.SetUniform("view", view);
    cube_map_shader_.SetUniform("projection", projection);

    // skybox cube
    glBindBuffer(GL_ARRAY_BUFFER, cube_map_vbo_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDepthFunc(GL_LESS); // set depth function back to default
#endif
    return true;
}

uint32_t OpenGLRenderCubeMap::LoadTexture(const std::string& texture_path) {
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

uint32_t OpenGLRenderCubeMap::LoadCubeMap(const std::vector<std::string>& files) {
    uint32_t texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    int width, height, channels;
    unsigned char *data;
    for(unsigned int i = 0; i < files.size(); i++) {
        data = stbi_load(files[i].c_str(), &width, &height, &channels, 0);
        if(!data) {
            std::cout << "stbi_load failed, filename: " << files[i] << std::endl;
            continue;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    return texture_id;
}

std::vector<float> OpenGLRenderCubeMap::GetCubeMapVertices() {
    std::vector<float> vertices{
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    return vertices;
}

std::vector<float> OpenGLRenderCubeMap::GetObjectVertices() {
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
