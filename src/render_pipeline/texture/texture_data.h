#pragma once

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include "common.h"
#include "pipeline_struct.h"
#include "texture_uncompress.h"
#include "texture.h"

class TextureData {
public:
    static TextureData& GetInstance();
    // void Clear();
    // void ClearTexture(int texture_name);
    void SetCurrentUseProgram(int program);
    // void InitTextureMap();
    void ClearTextureName();

    Color GetData(const std::string& name, float u, float v);
    Color GetCubeData(const std::string& name, float x, float y, float z);

    void CreateTexture(int texture_id);
    void DestoryTexture(int texture_id);
    void TexParameterf(int texture_id, GLenum pname, GLfloat param);
    void TexParameteri(int texture_id, GLenum pname, GLint param);
    void TexParameterfv(int texture_id, GLenum pname, const GLfloat * params);
    void TexParameteriv(int texture_id, GLenum pname, const GLint * params);
    void TexImage2D(int texture_id, GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * data);
    void TexSubImage2D(int texture_id, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void * data);
    void CompressedTexImage2D(int texture_id, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei imageSize, const void * data);
    void CompressedTexSubImage2D(int texture_id, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLsizei imageSize, const void * data);
    void CopyTexImage2D(int texture_id, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
    void CopyTexSubImage2D(int texture_id, GLint level, GLint xoffset,  GLint yoffset, GLint x, GLint y, GLsizei width,  GLsizei height);
    void PixelStorei(int texture_id, GLenum pname, GLint param);
    void GenerateMipmap(int texture_id);

    uint32_t GetData(int texture_id, int addr, int level);
    void SetData(int texture_id, int addr, int data, int level);

private:
    TextureData();
    ~TextureData();

    TextureData(const TextureData&) = delete;
    TextureData operator=(const TextureData&) = delete;

private:
    int current_use_program_{};
    std::unordered_map<int, std::shared_ptr<Texture>> texture_map_{};
    std::unordered_map<std::string, int> texture_id_map_{};
};