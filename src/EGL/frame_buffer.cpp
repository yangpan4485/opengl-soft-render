#include "frame_buffer.h"

#include <cmath>
#include <algorithm>
#include <iostream>

#include "global_config.h"
#include "texture/texture_data.h"

FrameBuffer::FrameBuffer() {
    // fout_.open("output.txt", std::ios::binary | std::ios::out);
}

FrameBuffer::~FrameBuffer() {
    // fout_.close();
}

FrameBuffer& FrameBuffer::GetInstance() {
    static FrameBuffer instance;
    return instance;
}

void FrameBuffer::SetFrameSize(uint16_t width, uint32_t height) {
    if (frame_width_ != width || frame_height_ != height) {
        frame_width_ = width;
        frame_height_ = height;

        std::vector<std::vector<uint32_t>> depth(width, std::vector<uint32_t>(height, 0));
        std::vector<std::vector<uint32_t>> stencil(width, std::vector<uint32_t>(height, 0));
        std::vector<std::vector<uint32_t>> pixel(width, std::vector<uint32_t>(height, 0));

        depth_buffer_.swap(depth);
        stencil_buffer_.swap(stencil);
        pixel_buffer_.swap(pixel);
    }
}

void FrameBuffer::ClearColor(uint32_t color) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        for(int i = 0; i < frame_width_; ++i) {
            for(int j = 0; j < frame_height_; ++j) {
                pixel_buffer_[i][j] = color;
            }
        }
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_texture.find(GL_COLOR_ATTACHMENT0) == context->frame_buffer_map[frame_buffer].frame_texture.end()) {
            return;
        }
        uint8_t r = color & 0xff;
        uint8_t g = (color & 0xff00) >> 8;
        uint8_t b = (color & 0xff0000) >> 16;
        uint8_t a = (color & 0xff000000) >> 24;
        int texture = context->frame_buffer_map[frame_buffer].frame_texture[GL_COLOR_ATTACHMENT0];
        int size = context->texture_map[texture].width * context->texture_map[texture].height;
        int stride = 4;
        if (context->texture_map[texture].format == GL_RGB) {
            stride = 3;
        }
        size = size * stride;
        for (int i = 0; i < size; i += stride) {
            TextureData::GetInstance().SetData(texture, i, r, 0);
            TextureData::GetInstance().SetData(texture, i + 1, g, 0);
            TextureData::GetInstance().SetData(texture, i + 2, b, 0);
            if (context->texture_map[texture].format == GL_RGBA) {
                TextureData::GetInstance().SetData(texture, i + 3, a, 0);
            }
        }
    }
}

void FrameBuffer::ClearDepth(uint32_t depth) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        for(int i = 0; i < frame_width_; ++i) {
            for(int j = 0; j < frame_height_; ++j) {
                depth_buffer_[i][j] = depth;
            }
        }
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_render_buffer.find(GL_DEPTH_ATTACHMENT) == context->frame_buffer_map[frame_buffer].frame_render_buffer.end()) {
            return;
        }
        int size = context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_DEPTH_ATTACHMENT].buffers.size();
        for (int i = 0; i < size; ++i) {
            context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_DEPTH_ATTACHMENT].buffers[i] = depth;
        }
    }
}

void FrameBuffer::ClearStencil(uint32_t stencil) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        for(int i = 0; i < frame_width_; ++i) {
            for(int j = 0; j < frame_height_; ++j) {
                stencil_buffer_[i][j] = stencil;
            }
        }
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_render_buffer.find(GL_STENCIL_INDEX8) == context->frame_buffer_map[frame_buffer].frame_render_buffer.end()) {
            return;
        }
        int size = context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_STENCIL_INDEX8].buffers.size();
        for (int i = 0; i < size; ++i) {
            context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_STENCIL_INDEX8].buffers[i] = stencil;
        }
    }
}

void FrameBuffer::SetDepth(uint16_t x, uint16_t y, uint32_t depth) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        depth_buffer_[x][y] = depth;
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_render_buffer.find(GL_DEPTH_ATTACHMENT) == context->frame_buffer_map[frame_buffer].frame_render_buffer.end()) {
            return;
        }
        int index = y * context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_DEPTH_ATTACHMENT].width + x;
        context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_DEPTH_ATTACHMENT].buffers[index] = depth;
    }
}

void FrameBuffer::SetStencil(uint16_t x, uint16_t y, uint32_t stencil) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        stencil_buffer_[x][y] = stencil;
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_render_buffer.find(GL_STENCIL_INDEX8) == context->frame_buffer_map[frame_buffer].frame_render_buffer.end()) {
            return;
        }
        int index = y * context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_STENCIL_INDEX8].width + x;
        context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_STENCIL_INDEX8].buffers[index] = stencil;
    }
}

void FrameBuffer::SetPixel(uint16_t x, uint16_t y, uint32_t pixel) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        pixel_buffer_[x][y] = pixel;
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_texture.find(GL_COLOR_ATTACHMENT0) == context->frame_buffer_map[frame_buffer].frame_texture.end()) {
            return;
        }
        uint8_t r = pixel & 0xff;
        uint8_t g = (pixel & 0xff00) >> 8;
        uint8_t b = (pixel & 0xff0000) >> 16;
        uint8_t a = (pixel & 0xff000000) >> 24;
        int texture = context->frame_buffer_map[frame_buffer].frame_texture[GL_COLOR_ATTACHMENT0];
        // std::cout << "texture: " << texture << std::endl;
        int width = context->texture_map[texture].width;
        int height = context->texture_map[texture].height;
        int index = (y * width + x) * 3;
        if (context->texture_map[texture].format == GL_RGBA) {
            index = (y * width + x) * 4;
        }
        TextureData::GetInstance().SetData(texture, index, r, 0);
        TextureData::GetInstance().SetData(texture, index + 1, g, 0);
        TextureData::GetInstance().SetData(texture, index + 2, b, 0);
        if (context->texture_map[texture].format == GL_RGBA) {
            TextureData::GetInstance().SetData(texture, index + 3, a, 0);
        }
        
    }
}

uint32_t FrameBuffer::GetDepth(uint16_t x, uint16_t y) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        return depth_buffer_[x][y];
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_render_buffer.find(GL_DEPTH_ATTACHMENT) == context->frame_buffer_map[frame_buffer].frame_render_buffer.end()) {
            return 0;
        }
        int index = y * context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_DEPTH_ATTACHMENT].width + x;
        return context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_DEPTH_ATTACHMENT].buffers[index];
    }
    
}

uint32_t FrameBuffer::GetStencil(uint16_t x, uint16_t y) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        return stencil_buffer_[x][y];
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_render_buffer.find(GL_STENCIL_INDEX8) == context->frame_buffer_map[frame_buffer].frame_render_buffer.end()) {
            return 0;
        }
        int index = y * context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_STENCIL_INDEX8].width + x;
        return context->frame_buffer_map[frame_buffer].frame_render_buffer[GL_STENCIL_INDEX8].buffers[index];
    }
    
}

uint32_t FrameBuffer::GetPixel(uint16_t x, uint16_t y) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer == 0) {
        return pixel_buffer_[x][y];
    } else {
        if (context->frame_buffer_map[frame_buffer].frame_texture.find(GL_COLOR_ATTACHMENT0) == context->frame_buffer_map[frame_buffer].frame_texture.end()) {
            return 0;
        }
        int texture = context->frame_buffer_map[frame_buffer].frame_texture[GL_COLOR_ATTACHMENT0];
        int width = context->texture_map[texture].width;
        int height = context->texture_map[texture].height;
        int index = (y * width + x) * 3;
        if (context->texture_map[texture].format == GL_RGBA) {
            index = (y * width + x) * 4;
        }
        
        uint32_t r = TextureData::GetInstance().GetData(texture, index, 0);
        uint32_t g = TextureData::GetInstance().GetData(texture, index + 1, 0);
        uint32_t b = TextureData::GetInstance().GetData(texture, index + 2, 0);
        uint32_t a = 255;
        if (context->texture_map[texture].format == GL_RGBA) {
            a = TextureData::GetInstance().GetData(texture, index + 3, 0);
        }
        uint32_t color = r + (g << 8) + (b << 16) + (a << 24);
        return color;
    }
}

void FrameBuffer::Flush() {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int frame_buffer = context->current_bind_frame_buffer;
    if (frame_buffer != 0) {
        return;
    }
    if (frame_callback_) {
        frame_callback_(pixel_buffer_);
    }
#if 0
    for (uint32_t j = 0; j < frame_height_; ++j) {
        for (uint32_t i = 0; i < frame_width_; ++i) {
            uint32_t pixel = pixel_buffer_[i][j];
            uint32_t r = pixel & 0xff;
            uint32_t g = (pixel & 0xff00) >> 8;
            uint32_t b = (pixel & 0xff0000) >> 16;
            uint32_t a = (pixel & 0xff000000) >> 24;
            // fout_ << i + 1 << " " << j + 1 << " " << r << " " << g << " " << b << " " << a << std::endl;
        }
    }
#endif
}

void FrameBuffer::SetFrameCallback(FrameCallback callback) {
    frame_callback_ = callback;
}