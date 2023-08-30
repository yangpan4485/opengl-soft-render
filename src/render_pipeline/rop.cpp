#include "rop.h"

#include <iostream>
#include <cmath>
#include <algorithm>

#include "frame_buffer.h"
#include "global_config.h"
#include "pipeline_utils.h"

Rop::Rop() {
    clear_depth_ = pow(2, 24);
}

Rop::~Rop() {}

void Rop::ClearColor(float r, float g, float b, float a) {
    uint32_t clear_r = Min(r * 256, 255);
    uint32_t clear_g = Min(g * 256, 255);
    uint32_t clear_b = Min(b * 256, 255);
    uint32_t clear_a = Min(a * 256, 255);
    clear_color_ = clear_r + (clear_g << 8) + (clear_b << 16) + (clear_a << 24);
}

void Rop::ClearDepth(uint32_t depth) {
    // std::cout << "clear_depth_: " << clear_depth_ << std::endl;
    clear_depth_ = depth;
}
void Rop::ClearStencil(uint32_t stencil) {
    clear_stencil_ = stencil;
}

void Rop::Clear(int mask) {
    bool clear_depth = mask & GL_DEPTH_BUFFER_BIT;
    bool clear_stencil = mask & GL_STENCIL_BUFFER_BIT;
    bool clear_pixel = mask & GL_COLOR_BUFFER_BIT;
    if (clear_pixel) {
        FrameBuffer::GetInstance().ClearColor(clear_color_);
    }
    if (clear_depth) {
        FrameBuffer::GetInstance().ClearDepth(clear_depth_);
    }
    if (clear_stencil) {
        FrameBuffer::GetInstance().ClearStencil(clear_stencil_);
    }
}

void Rop::Flush() {
    FrameBuffer::GetInstance().Flush();
}

void Rop::SetBlendConfig(BlendConfig config) {
    blend_config_ = config;
}

void Rop::SetDepthConfig(DepthConfig config) {
    depth_config_ = config;
}

void Rop::SetStencilConfig(StencilConfig config) {
    stencil_config_ = config;
}

void Rop::SetScissorConfig(ScissorConfig config) {
    // std::cout << "SetScissorConfig" << std::endl;
    scissor_config_ = config;
}

void Rop::SetColorMask(ColorMask color_mask) {
    color_mask_ = color_mask;
}

void Rop::PushData(const PixelAttri& pixel) {
    // std::cout << "rop: " << pixel.x << " " << pixel.y << std::endl;
    bool scissor_pass = true;
    if (scissor_config_.enable_scissor) {
        // std::cout << "enable_scissor: " << scissor_config_.enable_scissor << std::endl;
        scissor_pass = ScissorTest(pixel);
    }
    if (!scissor_pass) {
        return;
    }
    bool depth_pass = true;
    if (depth_config_.enable_depth) {
        depth_pass = DepthTest(pixel);
    }
    if (depth_pass && depth_config_.mask == true) {
        FrameBuffer::GetInstance().SetDepth(pixel.x, pixel.y, pixel.z);
    }

    bool stencil_pass = true;
    if (stencil_config_.enable_stencil) {
        // std::cout << "StencilTest: " << std::endl;
        stencil_pass = StencilTest(pixel);
        int mask = pixel.front_face ? stencil_config_.front_mask : stencil_config_.back_mask;
        int sfail = pixel.front_face ? stencil_config_.front_sfail : stencil_config_.back_sfail;
        int dpfail = pixel.front_face ? stencil_config_.front_dpfail : stencil_config_.back_dpfail;
        int dppass = pixel.front_face ? stencil_config_.front_dppass : stencil_config_.back_dppass;
        if (mask == 0xff) {
            if (!stencil_pass) {
                UpdateStencilBuffer(sfail, pixel);
            } else {
                if (!depth_pass) {
                    UpdateStencilBuffer(dpfail, pixel);
                } else {
                    UpdateStencilBuffer(dppass, pixel);
                }
            }
        }
    }
    
    // std::cout << "depth_pass: " << depth_pass << std::endl;
    if (!depth_pass || !stencil_pass) {
        return;
    }
    uint32_t buffer_color = FrameBuffer::GetInstance().GetPixel(pixel.x, pixel.y);
    Color dst_color;
    dst_color.r = buffer_color & 0xff;
    dst_color.g = (buffer_color & 0xff00) >> 8;
    dst_color.b = (buffer_color & 0xff0000) >> 16;
    dst_color.a = (buffer_color & 0xff000000) >> 24;
    Color src_color;
    // std::cout << "blend_config_.enable_blend: " << blend_config_.enable_blend << std::endl;
    if (blend_config_.enable_blend) {
        src_color = BlendTest(pixel);
    } else {
        src_color.r = pixel.r;
        src_color.g = pixel.g;
        src_color.b = pixel.b;
        src_color.a = pixel.a;
    }
    Color result = CalcColorMask(src_color, dst_color);
    uint32_t color = result.r + (result.g << 8) + (result.b << 16) + (result.a << 24);
    
    // std::cout << "rop push data: " << pixel.x << " " << pixel.y << " " << (int)result.r << " " 
    //           << (int)result.g << " " << (int)result.b << " " << (int)result.a << " " << std::endl;
    FrameBuffer::GetInstance().SetPixel(pixel.x, pixel.y, color);
}

Color Rop::CalcColorMask(const Color& src, const Color& dst) {
    Color result;
    result.r = color_mask_.red ? src.r : dst.r;
    result.g = color_mask_.green ? src.g : dst.b;
    result.b = color_mask_.blue ? src.b : dst.b;
    result.a = color_mask_.alpha ? src.a : dst.a;
    return result;
}

void Rop::UpdateStencilBuffer(GLenum op_code, const PixelAttri& pixel) {
    // std::cout << "UpdateStencilBuffer" << std::endl;
    uint8_t stencil = FrameBuffer::GetInstance().GetStencil(pixel.x, pixel.y);
    switch (op_code) {
    case GL_KEEP:
        break;
    case GL_ZERO:
        FrameBuffer::GetInstance().SetStencil(pixel.x, pixel.y, 0);
        break;
    case GL_REPLACE: {
        int ref = pixel.front_face ? stencil_config_.front_stencil_func_ref : stencil_config_.back_stencil_func_ref;
        FrameBuffer::GetInstance().SetStencil(pixel.x, pixel.y, ref);
        break;
    }
    case GL_INCR: {
        if (stencil < 0xff) {
            stencil = stencil + 1;
        }
        FrameBuffer::GetInstance().SetStencil(pixel.x, pixel.y, stencil);
        break;
    }
    case GL_INCR_WRAP: {
        if (stencil < 0xff) {
            stencil = stencil + 1;
        } else {
            stencil = 0;
        }
        FrameBuffer::GetInstance().SetStencil(pixel.x, pixel.y, stencil);
        break;
    }
    case GL_DECR: {
        if (stencil > 0) {
            stencil = stencil - 1;
        }
        FrameBuffer::GetInstance().SetStencil(pixel.x, pixel.y, stencil);
        break;
    }
    case GL_DECR_WRAP: {
        if (stencil > 0) {
            stencil = stencil - 1;
        } else {
            stencil = 0xff;
        }
        FrameBuffer::GetInstance().SetStencil(pixel.x, pixel.y, stencil);
        break;
    }   
    case GL_INVERT: {
        stencil = (uint8_t)~stencil;
        FrameBuffer::GetInstance().SetStencil(pixel.x, pixel.y, stencil);
        break;
    }
    default:
        break;
    }
}

bool Rop::ScissorTest(const PixelAttri& pixel) {
    // std::cout << "ScissorTest" << std::endl;
    if (pixel.x < scissor_config_.scissor_x || pixel.x > (scissor_config_.scissor_x + scissor_config_.scissor_width)) {
        return false;
    }
    if (pixel.y < scissor_config_.scissor_y || pixel.y > (scissor_config_.scissor_y + scissor_config_.scissor_height)) {
        return false;
    }
    return true;
}

bool Rop::DepthTest(const PixelAttri& pixel) {
    bool depth_pass = true;
    uint32_t buffer_z = FrameBuffer::GetInstance().GetDepth(pixel.x, pixel.y);
    // std::cout << "buffer_z: " << buffer_z * 1.0 / pow(2, 24) << std::endl;
    // if (pixel.x == 384 && pixel.y == 223) {
    //     std::cout << "pixel.z: " << pixel.z * 1.0 / pow(2, 24) << std::endl;
    // }
    // std::cout << "depth_config_.func" << std::hex << depth_config_.func << std::endl;
    // std::cout << "pixel.z: " << pixel.z * 1.0 / pow(2, 24) << std::endl;
    switch(depth_config_.func) {
    case GL_NEVER:
        depth_pass = false;
        break;
    case GL_LESS:
        depth_pass = (pixel.z < buffer_z);
        break;
    case GL_EQUAL:
        depth_pass = (pixel.z == buffer_z);
        break;
    case GL_LEQUAL:
        depth_pass = (pixel.z <= buffer_z);
        break;
    case GL_GREATER:
        depth_pass = (pixel.z > buffer_z);
        break;
    case GL_NOTEQUAL:
        depth_pass = (pixel.z != buffer_z);
        break;
    case GL_GEQUAL:
        depth_pass = (pixel.z >= buffer_z);
        break;
    case GL_ALWAYS:
        depth_pass = true;
        break;
    default:
        break;
    }
    // if (depth_pass) {
    //     std::cout << "depth_pass: " << depth_pass << std::endl;
    // }
    
    return depth_pass;
}

bool Rop::StencilTest(const PixelAttri& pixel) {
    bool stencil_pass = true;
    int stencil_func = pixel.front_face ? stencil_config_.front_stencil_func : stencil_config_.back_stencil_func;;
    int stencil_func_ref = pixel.front_face ? stencil_config_.front_stencil_func_ref : stencil_config_.back_stencil_func_ref;
    int stencil_func_mask = pixel.front_face ? stencil_config_.front_stencil_func_mask : stencil_config_.back_stencil_func_mask;
    int stencil = static_cast<int>(FrameBuffer::GetInstance().GetStencil(pixel.x, pixel.y));
    stencil_func_ref = stencil_func_ref & stencil_func_mask;
    stencil = stencil & stencil_func_mask;
    switch (stencil_func) {
    case GL_NEVER: 
        stencil_pass = false;
        break;
    case GL_LESS:
        stencil_pass = stencil_func_ref < stencil;
        break;
    case GL_LEQUAL: 
        stencil_pass = stencil_func_ref <= stencil;
        break;
    case GL_GREATER: 
        stencil_pass = stencil_func_ref > stencil;
        break;
    case GL_GEQUAL: 
        stencil_pass = stencil_func_ref >= stencil;
        break;
    case GL_EQUAL: 
        stencil_pass = stencil_func_ref == stencil;
        break;
    case GL_NOTEQUAL: 
        stencil_pass = stencil_func_ref != stencil;
        break;
    case GL_ALWAYS: 
        stencil_pass = true;
        break;
    default:
        break;
    }
    
    return stencil_pass;
}

Color Rop::BlendTest(const PixelAttri& pixel) {
    uint32_t buffer_pixel = FrameBuffer::GetInstance().GetPixel(pixel.x, pixel.y);
    Color src;
    src.r = pixel.r;
    src.g = pixel.g;
    src.b = pixel.b;
    src.a = pixel.a;
    Color dst;
    dst.r = buffer_pixel & 0xff;
    dst.g = (buffer_pixel & 0xff00) >> 8;
    dst.b = (buffer_pixel & 0xff0000) >> 16;
    dst.a = (buffer_pixel & 0xff000000) >> 24;
    // std::cout << "src: " << (int)src.r << " " << (int)src.g << " " << (int)src.b << " " << (int)src.a << std::endl;
    // std::cout << "dst: " << (int)dst.r << " " << (int)dst.g << " " << (int)dst.b << " " << (int)dst.a << std::endl;
    std::vector<float> src_rgb_factor = CalcBlendFactorRGB(blend_config_.blend_func_src_rgb, src, dst);
    float src_alpha_factor = CalcBlendFactorAlpha(blend_config_.blend_func_src_alpha, src, dst);
    std::vector<float> dst_rgb_factor = CalcBlendFactorRGB(blend_config_.blend_func_dst_rgb, src, dst);
    float dst_alpha_factor = CalcBlendFactorAlpha(blend_config_.blend_func_dst_alpha, src, dst);
    Color result;
    result.r = CalcBlendEquation(blend_config_.blend_equation_rgb, src.r, src_rgb_factor[0], dst.r, dst_rgb_factor[0]);
    result.g = CalcBlendEquation(blend_config_.blend_equation_rgb, src.g, src_rgb_factor[1], dst.g, dst_rgb_factor[1]);
    result.b = CalcBlendEquation(blend_config_.blend_equation_rgb, src.b, src_rgb_factor[2], dst.b, dst_rgb_factor[2]);
    result.a = CalcBlendEquation(blend_config_.blend_equation_alpha, src.a, src_alpha_factor, dst.a, dst_alpha_factor);
    return result;
}

std::vector<float> Rop::CalcBlendFactorRGB(int func, const Color& src, const Color& dst) {
    std::vector<float> result(3, 1.0f);
    switch (func) {
    case GL_ZERO: 
        result[0] = 0.0f;
        result[1] = 0.0f;
        result[2] = 0.0f;
        break;
    case GL_ONE: 
        result[0] = 1.0f;
        result[1] = 1.0f;
        result[2] = 1.0f;
        break;
    case GL_SRC_COLOR: 
        result[0] = src.r * 1.0f / 255;
        result[1] = src.g * 1.0f / 255;
        result[2] = src.g * 1.0f / 255;
        break;
    case GL_ONE_MINUS_SRC_COLOR: 
        result[0] = 1 - src.r * 1.0f / 255;
        result[1] = 1 - src.g * 1.0f / 255;
        result[2] = 1 - src.g * 1.0f / 255;
        break;
    case GL_DST_COLOR: 
        result[0] = dst.r * 1.0f / 255;
        result[1] = dst.g * 1.0f / 255;
        result[2] = dst.g * 1.0f / 255;
        break;
    case GL_ONE_MINUS_DST_COLOR: 
        result[0] = 1 - dst.r * 1.0f / 255;
        result[1] = 1 - dst.g * 1.0f / 255;
        result[2] = 1 - dst.g * 1.0f / 255;
        break;
    case GL_SRC_ALPHA: 
        result[0] = src.a * 1.0 / 255;
        result[1] = src.a * 1.0 / 255;
        result[2] = src.a * 1.0 / 255;
        break;
    case GL_ONE_MINUS_SRC_ALPHA: 
        result[0] = 1 - src.a * 1.0 / 255;
        result[1] = 1 - src.a * 1.0 / 255;
        result[2] = 1 - src.a * 1.0 / 255;
        break;
    case GL_DST_ALPHA: 
        result[0] = dst.a * 1.0 / 255;
        result[1] = dst.a * 1.0 / 255;
        result[2] = dst.a * 1.0 / 255;
        break;
    case GL_ONE_MINUS_DST_ALPHA: 
        result[0] = 1 - dst.a * 1.0 / 255;
        result[1] = 1 - dst.a * 1.0 / 255;
        result[2] = 1 - dst.a * 1.0 / 255;
        break;
    case GL_CONSTANT_COLOR: 
        result[0] = blend_config_.blend_r;
        result[1] = blend_config_.blend_g;
        result[2] = blend_config_.blend_b;
        break;
    case GL_ONE_MINUS_CONSTANT_COLOR: 
        result[0] = 1 - blend_config_.blend_r;
        result[1] = 1 - blend_config_.blend_g;
        result[2] = 1 - blend_config_.blend_b;
        break;
    case GL_CONSTANT_ALPHA: 
        result[0] = blend_config_.blend_a;
        result[1] = blend_config_.blend_a;
        result[2] = blend_config_.blend_a;
        break;
    case GL_ONE_MINUS_CONSTANT_ALPHA: 
        result[0] = 1 - blend_config_.blend_a;
        result[1] = 1 - blend_config_.blend_a;
        result[2] = 1 - blend_config_.blend_a;
        break;
    default:
        break;
    }
    return result;
}

float Rop::CalcBlendFactorAlpha(int func, const Color& src, const Color& dst) {
    float factor = 0.0f;
    switch (func) {
    case GL_ZERO: 
        factor = 0.0f;
        break;
    case GL_ONE: 
        factor = 1.0f;
        break;
    case GL_SRC_COLOR: 
        break;
    case GL_ONE_MINUS_SRC_COLOR: 
        break;
    case GL_DST_COLOR: 
        break;
    case GL_ONE_MINUS_DST_COLOR: 
        break;
    case GL_SRC_ALPHA: 
        factor = src.a * 1.0 / 255;
        break;
    case GL_ONE_MINUS_SRC_ALPHA: 
        factor = 1 - (src.a * 1.0 / 255);
        break;
    case GL_DST_ALPHA: 
        factor = dst.a * 1.0 / 255;
        break;
    case GL_ONE_MINUS_DST_ALPHA: 
        factor = 1 - (dst.a * 1.0 / 255);
        break;
    case GL_CONSTANT_COLOR: 
        break;
    case GL_ONE_MINUS_CONSTANT_COLOR: 
        break;
    case GL_CONSTANT_ALPHA: 
        factor = blend_config_.blend_a;
        break;
    case GL_ONE_MINUS_CONSTANT_ALPHA: 
        factor = 1 - blend_config_.blend_a;
        break;
    default:
        break;
    }
    return factor;
}

uint8_t Rop::CalcBlendEquation(int func, uint8_t src, float src_factor, uint8_t dst, float dst_factor) {
    uint8_t result = 0;
    switch (func) {
    case GL_FUNC_ADD:
        result = src * src_factor + dst * dst_factor;
        break;
    case GL_FUNC_SUBTRACT:
        result = src * src_factor - dst * dst_factor;
        break;
    case GL_FUNC_REVERSE_SUBTRACT:
        result = dst * dst_factor - src * src_factor;
        break;
    default:
        break;
    }

    return result;
}