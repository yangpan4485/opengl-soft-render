#include "gl_texture.h"

#include <iostream>
#include <fstream>

#include "global_config.h"
#include "frame_buffer.h"
#include "texture/texture_data.h"

void glGenTextures(GLsizei n, GLuint * textures) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (n < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    for(int i = 0; i < n; ++i) {
        GLuint texture;
        std::unique_lock<std::mutex> lock(context->texture_mtx);
        if (context->current_texture_name < context->max_texture_name) {
            texture = context->current_texture_name;
            context->current_texture_name++;
        } else {
            for(int j = 1; j < context->max_texture_name; ++j) {
                if (context->texture_map.find(j) != context->texture_map.end()) {
                    texture = j;
                    break;
                }
            }
        }
        TextureObject texture_object;
        texture_object.texture_name = texture;
        context->texture_map[texture] = texture_object;
        TextureData::GetInstance().CreateTexture(texture);
        if ( n == 1) {
            *textures = texture;
        } else {
            textures[i] = texture;
        }
    }
}

void glBindTexture(GLenum target, GLuint texture){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->texture_map.find(texture) == context->texture_map.end()) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    context->active_texture_map[context->current_active_texture] = texture;
    context->bind_texture_map[target] = texture;
    context->current_bind_texture_target = target;
    TextureData::GetInstance().ClearTextureName();
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, 
                  GLenum format, GLenum type, const void * data) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP_POSITIVE_X &&
        target != GL_TEXTURE_CUBE_MAP_NEGATIVE_X && target != GL_TEXTURE_CUBE_MAP_POSITIVE_Y && 
        target != GL_TEXTURE_CUBE_MAP_NEGATIVE_Y && target != GL_TEXTURE_CUBE_MAP_POSITIVE_Z && 
        target != GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (format != GL_ALPHA && format != GL_RGB && format != GL_RGBA && 
        format != GL_LUMINANCE && format != GL_LUMINANCE_ALPHA) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (type != GL_UNSIGNED_BYTE && type != GL_UNSIGNED_SHORT_5_6_5 && 
        type != GL_UNSIGNED_SHORT_4_4_4_4 && type != GL_UNSIGNED_SHORT_5_5_5_1) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (level < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (target != GL_TEXTURE_2D && width != height) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (internalformat != GL_ALPHA && internalformat != GL_RGB && internalformat != GL_RGBA && 
        internalformat != GL_LUMINANCE && internalformat != GL_LUMINANCE_ALPHA) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (width < 0 || height < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (border != 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (format != internalformat) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    if (type == GL_UNSIGNED_SHORT_5_6_5 && format != GL_RGB) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    if ((type == GL_UNSIGNED_SHORT_4_4_4_4 || type == GL_UNSIGNED_SHORT_5_5_5_1) && format != GL_RGBA) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }

    int texture = context->bind_texture_map[context->current_bind_texture_target];
    context->texture_map[texture].level = level;
    // GL_RGB, GL_RGBA
    context->texture_map[texture].internal_format = internalformat;
    context->texture_map[texture].width = width;
    context->texture_map[texture].height = height;
    // GL_RGB, GL_RGBA
    context->texture_map[texture].format = format;
    // GL_UNSIGNED_BYTE
    context->texture_map[texture].type = type;
    int index = level;
    if (context->current_bind_texture_target == GL_TEXTURE_CUBE_MAP) {
        index = target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }
    TextureData::GetInstance().TexImage2D(texture, index, width, height, format, type, data);
}


void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, 
                     GLenum format, GLenum type, const void * data) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    TextureData::GetInstance().TexSubImage2D(texture, level, xoffset, yoffset, width, height, data);
}

void glTexParameterf(GLenum target, GLenum pname, GLfloat param){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    context->texture_map[texture].params_map[pname] = (int)(param);
    TextureData::GetInstance().TexParameterf(texture, pname, param);
}

void glTexParameteri(GLenum target, GLenum pname, GLint param){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    context->texture_map[texture].params_map[pname] = (int)(param);
    TextureData::GetInstance().TexParameteri(texture, pname, param);
}

void glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params){
    // TODO
}

void glTexParameteriv(GLenum target, GLenum pname, const GLint * params){
    // TODO
}

void glActiveTexture(GLenum texture){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    context->current_active_texture = texture;
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, 
                            GLsizei height, GLint border, GLsizei imageSize, const void * data){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    context->texture_map[texture].internal_format = internalformat;
    context->texture_map[texture].format = internalformat;
    context->texture_map[texture].width = width;
    context->texture_map[texture].height = height;
    TextureData::GetInstance().CompressedTexImage2D(texture, level, internalformat, width, height, imageSize, data);
}

void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,  GLint yoffset, GLsizei width, 
                               GLsizei height,  GLenum format, GLsizei imageSize,  const void * data){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    TextureData::GetInstance().CompressedTexSubImage2D(texture, level, xoffset, yoffset, width, height, imageSize, data);
}

void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, 
                      GLsizei height, GLint border){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    int index = 0;
    int size = width * height * 3;
    if (internalformat == GL_RGBA) {
        size = width * height * 4;
    }
    
    context->texture_map[texture].level = level;
    // GL_RGB, GL_RGBA
    context->texture_map[texture].internal_format = internalformat;
    context->texture_map[texture].format = internalformat;
    context->texture_map[texture].width = width;
    context->texture_map[texture].height = height;
    TextureData::GetInstance().CopyTexImage2D(texture, level, internalformat, x, y, width, height);
}

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,  GLint yoffset, GLint x, GLint y, GLsizei width, 
                         GLsizei height){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    TextureData::GetInstance().CopyTexSubImage2D(texture, level, xoffset, yoffset, x, y, width, height);
}

void glGenerateMipmap(GLenum target){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    TextureData::GetInstance().GenerateMipmap(texture);
    // TODO
}

void glDeleteTextures(GLsizei n, const GLuint* textures){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (n < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    for(int i = 0; i < n; ++i) {
        std::unique_lock<std::mutex> lock(context->texture_mtx);
        if (textures[i] == context->current_texture_name) {
            context->current_texture_name--;
        }
        TextureData::GetInstance().DestoryTexture(textures[i]);
        context->texture_map.erase(textures[i]);
    }
}

void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    *params = context->texture_map[texture].params_map[pname];
}

void glGetTexParameteriv(GLenum target, GLenum pname, GLint * params){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    *params = context->texture_map[texture].params_map[pname];
}

void glPixelStorei(GLenum pname, GLint param){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int texture = context->bind_texture_map[context->current_bind_texture_target];
    if (pname == GL_PACK_ALIGNMENT) {
        context->texture_map[texture].pixel_storei = param;
    } else if (pname == GL_UNPACK_ALIGNMENT) {
        context->texture_map[texture].pixel_storei = param;
    }
}

GLboolean glIsTexture(GLuint texture){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    // std::cout << "context->texture_map: " << context->texture_map.size() << std::endl;
    // for(const auto& val : context->texture_map) {
    //     std::cout << val.first << std::endl;
    // }
    if (context->texture_map.find(texture) != context->texture_map.end()) {
        return true;
    }
    return false;
}