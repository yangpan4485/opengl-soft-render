#include "texture_data.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include "common.h"
#include "uniform.h"
#include "global_config.h"
#include "utils.h"

TextureData& TextureData::GetInstance() {
    static TextureData insatnce;
    return insatnce;
}

TextureData::TextureData() {
}

TextureData::~TextureData() {
}

void TextureData::CreateTexture(int texture_id) {
    if (texture_map_.find(texture_id) == texture_map_.end()) {
        texture_map_[texture_id].reset(new Texture());
    }
}

void TextureData::DestoryTexture(int texture_id) {
    if (texture_map_.find(texture_id) != texture_map_.end()) {
        texture_map_.erase(texture_id);
    }
}

void TextureData::TexParameterf(int texture_id, GLenum pname, GLfloat param) {
    texture_map_[texture_id]->SetMode(pname, (int)param);
}

void TextureData::TexParameteri(int texture_id, GLenum pname, GLint param) {
    // std::cout << "texture_id: " << texture_id << std::endl;
    texture_map_[texture_id]->SetMode(pname, param);
}

void TextureData::TexParameterfv(int texture_id, GLenum pname, const GLfloat * params) {
}

void TextureData::TexParameteriv(int texture_id, GLenum pname, const GLint * params) {
}

void TextureData::TexImage2D(int texture_id, GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * data) {
    texture_map_[texture_id]->TexImage2D(level, width, height, format, type, data);
}

void TextureData::TexSubImage2D(int texture_id, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void * data) {
    texture_map_[texture_id]->TexSubImage2D(level, xoffset, yoffset, width, height, data);
}

void TextureData::CompressedTexImage2D(int texture_id, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei imageSize, const void * data) {
    texture_map_[texture_id]->CompressedTexImage2D(level, internalformat, width, height,imageSize, data);
}

void TextureData::CompressedTexSubImage2D(int texture_id, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLsizei imageSize, const void * data) {
    texture_map_[texture_id]->CompressedTexSubImage2D(level, xoffset, yoffset, width, height, imageSize, data);
}

void TextureData::CopyTexImage2D(int texture_id, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height) {
    texture_map_[texture_id]->CopyTexImage2D(level, internalformat, x, y, width, height);
}

void TextureData::CopyTexSubImage2D(int texture_id, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width,  GLsizei height) {
    texture_map_[texture_id]->CopyTexSubImage2D(level, xoffset, yoffset, x, y, width, height);
}

void TextureData::PixelStorei(int texture_id, GLenum pname, GLint param) {
    texture_map_[texture_id]->PixelStorei(pname, param);
}

void TextureData::GenerateMipmap(int texture_id) {
    texture_map_[texture_id]->GenerateMipmap();
}

uint32_t TextureData::GetData(int texture_id, int addr, int level) {
    return texture_map_[texture_id]->GetData(addr, level);
}

void TextureData::SetData(int texture_id, int addr, int data, int level) {
    texture_map_[texture_id]->SetData(addr, data, level);
}

void TextureData::SetCurrentUseProgram(int program) {
    current_use_program_ = program;
}

void TextureData::ClearTextureName() {
    texture_id_map_.clear();
}

Color TextureData::GetData(const std::string& name, float u, float v) {
    if (texture_id_map_.find(name) == texture_id_map_.end()) {
        std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
        int uniform_location = context->program_map[current_use_program_].uniform_location_map[name];
        std::vector<float> uniform_val = Uniform::GetInstance().GetValue(current_use_program_, uniform_location);
        int texture_id = uniform_val[0];
        int texture_name = context->active_texture_map[texture_id + GL_TEXTURE0];
        texture_id_map_[name] = texture_name;
        // std::cout << name << " " << texture_id_map_[name] << std::endl;
    }
    return texture_map_[texture_id_map_[name]]->GetData(u, v, 0);
}

Color TextureData::GetCubeData(const std::string& name, float x, float y, float z) {
    if (texture_id_map_.find(name) == texture_id_map_.end()) {
        std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
        int uniform_location = context->program_map[current_use_program_].uniform_location_map[name];
        std::vector<float> uniform_val = Uniform::GetInstance().GetValue(current_use_program_, uniform_location);
        int texture_id = uniform_val[0];
        int texture_name = context->active_texture_map[texture_id + GL_TEXTURE0];
        texture_id_map_[name] = texture_name;
    }
    int index = 0;
    float u, v;
    float mag = std::max(std::max(fabs(x), fabs(y)), fabs(z));
    if (mag == fabs(x)) {
        index = x >= 0 ? 0 : 1;
        if (x >= 0) {
            u = (-z / mag + 1.0);
            v = (-y / mag + 1.0);
        } else {
            u = (z / mag + 1.0);
            v = (-y / mag + 1.0);
        }
    } else if (mag == fabs(y)) {
        index = y >= 0 ? 2 : 3;
        if (y >= 0) {
            u = (x / mag + 1.0);
            v = (z / mag + 1.0);
        } else {
            u = (x / mag + 1.0);
            v = (-z / mag + 1.0);
        }
    } else if (mag == fabs(z)) {
        index = z >= 0 ? 4 : 5;
        if (z >= 0) {
            u = (x / mag + 1.0);
            v = (-y / mag + 1.0);
        } else {
            u = (-x / mag + 1.0);
            v = (-y / mag + 1.0);
        }
    }
    u = u * 0.5;
    v = v * 0.5;
    return texture_map_[texture_id_map_[name]]->GetData(u, v, index);
}