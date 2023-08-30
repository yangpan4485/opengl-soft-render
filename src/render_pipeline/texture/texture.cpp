#include "texture.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include "common.h"
#include "uniform.h"
#include "global_config.h"
#include "utils.h"
#include "frame_buffer.h"

Color lerp(Color color_a, Color color_b, float weight) {
    Color out;
    out.r = color_a.r * (1.0f - weight) + color_b.r * weight;
    out.g = color_a.g * (1.0f - weight) + color_b.g * weight;
    out.b = color_a.b * (1.0f - weight) + color_b.b * weight;
    out.a = color_a.a * (1.0f - weight) + color_b.a * weight;
    return out;
}

Texture::Texture() {
}

Texture::~Texture() {
}

void Texture::SetMode(int key, int value) {
    mode_map_[key] = value;
}

void Texture::TexImage2D(GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * data) {
    width_ = width;
    height_ = height;
    format_ = format;
    level_ = level;
    type_ = type;
    int size = 0;
    if (format == GL_RGB) {
        size = width * height * 3;
    } else if (format == GL_RGBA) {
        size = width * height * 4;
    } else if (format == GL_LUMINANCE) {
        size = width * height;
    }
    if (data_map_[level].size() != size) {
        data_map_[level].resize(size);
    }
    if (data == nullptr) {
        return;
    }
    uint8_t* image = (uint8_t*)data;
    for (int i = 0; i < size; ++i) {
        data_map_[level][i] = image[i];
    }
}

void Texture::TexSubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void * data) {
    int tex_width = width_;
    int tex_height = height_;
    uint8_t* image = (uint8_t*)data;
    int bit_wide = 0;
    if (format_ == GL_RGB) {
        bit_wide = 3;
    } else if (format_ == GL_RGBA) {
        bit_wide = 4;
    }
    for (int i = 0; i < height; ++i) {
        int index = ((yoffset + i) * tex_width + xoffset) * bit_wide;
        for (int j = 0; j < width * bit_wide; ++j) {
            data_map_[level][index + j] = image[i * height * bit_wide + j];
        }
    }
}

void Texture::CompressedTexImage2D(GLint level, GLenum format, GLsizei width, GLsizei height, GLsizei imageSize, const void * data) {
    width_ = width;
    height_ = height;
    format_ = format;
    level_ = level;
    int size = 0;
    if (format == GL_COMPRESSED_RGB8_ETC2) {
        size = width * height / 2;
    } else if (format == GL_COMPRESSED_RGBA8_ETC2_EAC) {
        size = width * height;
    }
    if (compress_data_.size() != size) {
        compress_data_.resize(size);
    }
    if (data == nullptr) {
        return;
    }
    uint8_t* image = (uint8_t*)data;
    for (int i = 0; i < imageSize; ++i) {
        compress_data_[i] = image[i];
    }
}

void Texture::CompressedTexSubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLsizei imageSize, const void * data) {
    if (compress_data_.size() == 0) {
        return;
    }
    xoffset = xoffset - xoffset % 4;
    yoffset = yoffset - yoffset % 4;
    int compress_index = 0;
    uint8_t* image = (uint8_t*)data;
    for (int i = 0; i < height; i += 4) {
        if (yoffset + i > height_) {
            continue;
        }
        int index = width_ * (yoffset + i) + xoffset * 4;
        if (format_ == GL_COMPRESSED_RGB8_ETC2) {
            index = index / 2;
        }
        for (int j = 0; j < width; j += 4) {
            if (xoffset + j > width_) {
                continue;
            }
            if (format_ == GL_COMPRESSED_RGB8_ETC2) {
                for (int k = 0; k < 8; ++k) {
                    compress_data_[index++] = image[compress_index++];
                }
            } else if (format_ == GL_COMPRESSED_RGBA8_ETC2_EAC) {
                for (int k = 0; k < 16; ++k) {
                    compress_data_[index++] = image[compress_index++];
                }
            }
        }
    }
}

void Texture::CopyTexImage2D(GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height) {
    int index = 0;
    int size = width * height * 3;
    if (internalformat == GL_RGBA) {
        size = width * height * 4;
    }
    if (data_map_[level].size() != size) {
        data_map_[level].resize(size);
    }
    width_ = width;
    height_ = height;
    format_ = internalformat;
    level_ = level;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uint32_t pixel = FrameBuffer::GetInstance().GetPixel(x + j, y + i);
            uint32_t r = pixel & 0xff;
            uint32_t g = (pixel & 0xff00) >> 8;
            uint32_t b = (pixel & 0xff0000) >> 16;
            uint32_t a = (pixel & 0xff000000) >> 24;
            if (internalformat == GL_RGB || internalformat == GL_RGBA) {
                data_map_[level][index++] = r;
                data_map_[level][index++] = g;
                data_map_[level][index++] = b;
                if (internalformat == GL_RGBA) {
                    data_map_[level][index++] = a;
                }
            }
        }
    }
}

void Texture::CopyTexSubImage2D(GLint level, GLint xoffset,  GLint yoffset, GLint x, GLint y, GLsizei width,  GLsizei height) {
    int bit_wide = 0;
    if (format_ == GL_RGB) {
        bit_wide = 3;
    } else if (format_ == GL_RGBA) {
        bit_wide = 4;
    }
    for (int i = 0; i < height; ++i) {
        int index = ((yoffset + i) * width_ + xoffset) * bit_wide;
        for (int j = 0; j < width; ++j) {
            uint32_t pixel = FrameBuffer::GetInstance().GetPixel(x + j, y + i);
            uint32_t r = pixel & 0xff;
            uint32_t g = (pixel & 0xff00) >> 8;
            uint32_t b = (pixel & 0xff0000) >> 16;
            uint32_t a = (pixel & 0xff000000) >> 24;
            if (format_ == GL_RGB || format_ == GL_RGBA) {
                data_map_[level][index++] = r;
                data_map_[level][index++] = g;
                data_map_[level][index++] = b;
                if (format_ == GL_RGBA) {
                    data_map_[level][index++] = a;
                }
            }
        }
    }
}


void Texture::PixelStorei(GLenum pname, GLint param) {
    pixel_storei_ = param;
}

void Texture::GenerateMipmap() {
    if (format_ == GL_COMPRESSED_RGB8_ETC2 || format_ == GL_COMPRESSED_RGBA8_ETC2_EAC) {
        if (data_map_[0].empty() && compress_data_.size() > 0) {
            int size = width_ * height_ * 4;
            data_map_[0].resize(size);
            texture_uncompress_.Decode(compress_data_, width_, height_, format_, data_map_[0]);
        }
        format_ = GL_RGBA;
    }
}

uint32_t Texture::GetData(int addr, int level) {
    return data_map_[level][addr];
}

void Texture::SetData(int addr, int data, int level) {
    data_map_[level][addr] = data;
}

Color Texture::GetData(float u, float v, int level) {
    u = CalcPosition(u, mode_map_[GL_TEXTURE_WRAP_S]);
    v = CalcPosition(v, mode_map_[GL_TEXTURE_WRAP_T]);
    if (mode_map_[GL_TEXTURE_MAG_FILTER] == GL_LINEAR ||
        mode_map_[GL_TEXTURE_MIN_FILTER] == GL_LINEAR) {
        float f_x = u * width_ - 0.5;
        float f_y = v * height_ - 0.5;
        float f_u = f_x - floor(f_x);
        float f_v = f_y - floor(f_y);
        int i_x = (int)(f_x) % width_;
        int i_y = (int)(f_y) % height_;
        if (i_x == width_ - 1) {
            i_x = i_x - 1;
        }
        if (i_y == height_ - 1) {
            i_y = i_y - 1;
        }
        i_x = i_x < 0 ? width_ + i_x : i_x;
        i_y = i_y < 0 ? height_ + i_y : i_y;
        int addr = i_x + i_y * width_;
        Color color[4];
        color[0] = GetColor(addr, format_, level);
        addr = (i_x + 1) + i_y * width_;
        color[1] = GetColor(addr, format_, level);
        addr = i_x + (i_y + 1) * width_;
        color[2] = GetColor(addr, format_, level);
        addr = (i_x + 1) + (i_y + 1) * width_;
        color[3] = GetColor(addr, format_, level);
        Color tmp_0 = lerp(color[0], color[1], f_u);
        Color tmp_1 = lerp(color[2], color[3], f_u);
        Color out_color = lerp(tmp_0, tmp_1, f_v);
        return out_color;
    }
    int i_u = (int)(u * width_ - 0.5f) % width_;
    int i_v = (int)(v * height_ - 0.5f) % height_;
    i_u = i_u < 0 ? width_ + i_u : i_u;
    i_v= i_v < 0 ? height_ + i_v : i_v;
    uint32_t addr = i_u + i_v * width_;
    return GetColor(addr, format_, level);
}


float Texture::CalcPosition(float pos, int mode) {
    int val = (int)pos;
    if (mode == GL_MIRRORED_REPEAT) {
        if (val % 2 == 0) {
            if (pos > 1.0) {
                pos = pos - val;
            }
        } else {
            pos = 1 - (pos - val);
        }
    }
    if (mode == GL_CLAMP_TO_EDGE) {
        if (pos > 1.0) {
            pos = 1.0;
        }
    }
    return pos;
}

Color Texture::GetColor(int index, int format, int level) {
    Color result;
    if (format == GL_RGB) {
        index = index * 3;
        // std::cout << "size: " << data_map_[level].size() << std::endl;
        result.r = data_map_[level][index];
        result.g = data_map_[level][index + 1];
        result.b = data_map_[level][index + 2];
        result.a = 255;
    } else if (format == GL_RGBA) {
        index = index * 4;
        result.r = data_map_[level][index];
        result.g = data_map_[level][index + 1];
        result.b = data_map_[level][index + 2];
        result.a = data_map_[level][index + 3];
    } else if (format == GL_LUMINANCE) {
        result.r = data_map_[level][index];
        result.g = data_map_[level][index];
        result.b = data_map_[level][index];
        result.a = 255;
    }
    return result;
}