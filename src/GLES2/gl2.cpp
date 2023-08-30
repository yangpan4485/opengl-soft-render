#include "gl2.h"

#include <iostream>
#include <sstream>

#include "global_config.h"
#include "render_pipeline/rop.h"
#include "gl_utils.h"
#include "render_pipeline/global_instance.h"
#include "frame_buffer.h"

void glFlush() {
    std::shared_ptr<Rop> rop_ins = GlobalInstance::GetInstance().GetRopInstance();
    rop_ins->Flush();
}

void glFinish() {
    std::shared_ptr<Rop> rop_ins = GlobalInstance::GetInstance().GetRopInstance();
    rop_ins->Flush();
}

void glLineWidth(float width) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    context->raster_config.linewidth = width;
}

void glEnable(GLenum cap) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    switch (cap) {
    case GL_BLEND:
        if (context->blend_config.enable_blend != true) {
            context->blend_config.enable_blend = true;
            context->blend_config.update_state |= 0b1000;
        }
        break;
    case GL_CULL_FACE:
        if (context->cullface_config.enable != true) {
            context->cullface_config.enable = true;
            context->cullface_config.update_state |= 0b01;
        }
        break;
    case GL_DEPTH_TEST:
        if (context->depth_config.enable_depth != true) {
            context->depth_config.enable_depth = true;
            context->depth_config.update_state |= 0b100;
        }
        break;
    case GL_DITHER:
        break;
    case GL_POLYGON_OFFSET_FILL:
        break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
        break;
    case GL_SAMPLE_COVERAGE:
        break;
    case GL_SCISSOR_TEST:
        if (context->scissor_config.enable_scissor != true) {
            context->scissor_config.enable_scissor = true;
            context->scissor_config.update_state |= 0b10;
        }
        break;
    case GL_STENCIL_TEST:
        if (context->stencil_config.enable_stencil != true) {
            context->stencil_config.enable_stencil = true;
            context->stencil_config.update_state |= 0b100000;
        }
        break;
    default:
        break;
    }
}

void glDisable(GLenum cap) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    switch (cap) {
    case GL_BLEND:
        if (context->blend_config.enable_blend != false) {
            context->blend_config.enable_blend = false;
            context->blend_config.update_state |= 0b1000;
        }
        break;
    case GL_CULL_FACE:
        if (context->cullface_config.enable != false) {
            context->cullface_config.enable = false;
            context->cullface_config.update_state |= 0b01;
        }
        break;
    case GL_DEPTH_TEST:
        if (context->depth_config.enable_depth != false) {
            context->depth_config.enable_depth = false;
            context->depth_config.update_state |= 0b100;
        }
        break;
    case GL_DITHER:
        break;
    case GL_POLYGON_OFFSET_FILL:
        break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
        break;
    case GL_SAMPLE_COVERAGE:
        break;
    case GL_SCISSOR_TEST:
        if (context->scissor_config.enable_scissor != false) {
            context->scissor_config.enable_scissor = false;
            context->scissor_config.update_state |= 0b10;
        }
        break;
    case GL_STENCIL_TEST:
        if (context->stencil_config.enable_stencil != false) {
            context->stencil_config.enable_stencil = false;
            context->stencil_config.update_state |= 0b100000;
        }
        break;
    default:
        break;
    }
}

GLboolean glIsEnabled (GLenum cap) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    switch (cap) {
    case GL_BLEND:
        return context->blend_config.enable_blend;
        break;
    case GL_CULL_FACE:
        return context->cullface_config.enable;
        break;
    case GL_DEPTH_TEST:
        return context->depth_config.enable_depth;
        break;
    case GL_DITHER:
        break;
    case GL_POLYGON_OFFSET_FILL:
        break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
        break;
    case GL_SAMPLE_COVERAGE:
        break;
    case GL_SCISSOR_TEST:
        return context->scissor_config.enable_scissor;
        break;
    case GL_STENCIL_TEST:
        return context->stencil_config.enable_stencil;
        break;
    default:
        break;
    }
    return false;
}

void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    context->color_mask.red = red;
    context->color_mask.green = green;
    context->color_mask.blue = blue;
    context->color_mask.alpha = alpha;
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->scissor_config.scissor_x != x || context->scissor_config.scissor_y != y ||
        context->scissor_config.scissor_width != width || context->scissor_config.scissor_height != height) {
        context->scissor_config.scissor_x = x;
        context->scissor_config.scissor_y = y;
        context->scissor_config.scissor_width = width;
        context->scissor_config.scissor_height = height;
        context->scissor_config.update_state |= 0b01;
    }
}

void glHint(GLenum target, GLenum mode) {
    // mipmap
}

void glPolygonOffset(GLfloat factor, GLfloat units) {
    // z-fighting
}

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * data) {
    uint8_t* buffer_data = (uint8_t*)data;
    int index = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uint32_t pixel = FrameBuffer::GetInstance().GetPixel(x + j, y + i);
            uint32_t r = pixel & 0xff;
            uint32_t g = (pixel & 0xff00) >> 8;
            uint32_t b = (pixel & 0xff0000) >> 16;
            uint32_t a = (pixel & 0xff000000) >> 24;
            if (format == GL_RGB || format == GL_RGBA) {
                buffer_data[index++] = r;
                buffer_data[index++] = g;
                buffer_data[index++] = b;
                if (format == GL_RGBA) {
                    buffer_data[index++] = a;
                }
            }
        }
    }
}

void glSampleCoverage(GLclampf value, GLboolean invert) {
    
}