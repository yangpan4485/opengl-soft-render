#pragma once
#include "gl_common.h"

struct RenderBufferObject {
    int render_buffer_name;
    int render_buffer_type;

    std::vector<uint32_t> buffers;
    int width;
    int height;
    int format;
};

struct FrameBufferObject {
    int frame_buffer_name;
    int frame_buffer_type;

    // std::unordered_map<int, TextureObject> frame_texture;
    std::unordered_map<int, int> frame_texture;
    std::unordered_map<int, RenderBufferObject> frame_render_buffer;
};