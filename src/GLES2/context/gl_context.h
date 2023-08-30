#pragma once
#include "gl_common.h"
#include "shader_object.h"
#include "program_object.h"
#include "buffer_object.h"
#include "texture_object.h"
#include "framebuffer_object.h"
#include "gl_config.h"

#define TEXTURE_LOCATION 1024

struct Context {
    // config
    ViewportConfig viewport_config;
    ClearConfig clear_config;
    RasterConfig raster_config;

    BlendConfig blend_config;
    CullfaceConfig cullface_config;
    DepthConfig depth_config;
    StencilConfig stencil_config;
    ScissorConfig scissor_config;

    // texture
    std::unordered_map<int, TextureObject> texture_map;
    std::unordered_map<int, int> bind_texture_map;
    std::unordered_map<int, int> active_texture_map;
    int current_bind_texture_target;
    int current_active_texture{0};
    std::mutex texture_mtx;
    int max_texture_name{INT32_MAX};
    int current_texture_name{1};
    int max_texture_size{4096};

    // shader
    std::mutex shader_mtx;
    int max_shader_name{INT32_MAX};
    int current_shader_name{1};
    std::unordered_map<int, ShaderObject> shader_map;

    // program
    std::mutex program_mtx;
    int max_program_name{INT32_MAX};
    int current_program_name{1};
    std::unordered_map<int, ProgramObject> program_map;
    int current_use_program;

    // frame buffer
    std::mutex frame_buffer_mtx;
    int max_frame_buffer_name{INT32_MAX};
    int current_frame_buffer_name{1};
    int current_bind_frame_buffer;
    std::unordered_map<int, FrameBufferObject> frame_buffer_map;

    // FrameBufferTexture frame_buffer_texture;

    // render buffer
    std::mutex render_buffer_mtx;
    int max_render_buffer_name{INT32_MAX};
    int current_render_buffer_name{1};
    int current_bind_render_buffer;
    std::unordered_map<int, RenderBufferObject> render_buffer_map;

    // buffer
    std::mutex buffer_mtx;
    int max_buffer_name{INT32_MAX};
    int current_buffer_name{1};
    std::unordered_map<int, BufferObject> buffer_map;
    std::unordered_map<int, int> current_bind_buffer_map;
    int current_buffer;
    
    // attribute 根据索引对应 vbo，如果后面设置的索引和前面相同会出现前面的索引读到的是后面的数据
    std::unordered_map<int, int> attrib_index_map;

    // vertex data;
    // std::unordered_map<int, VertexData> data_map;
    int max_attribute_num{16};

    int error_code{GL_NO_ERROR};

    // color mask
    ColorMask color_mask;
};