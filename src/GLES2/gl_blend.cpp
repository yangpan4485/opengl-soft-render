#include "gl_blend.h"

#include "global_config.h"
#include "gl_utils.h"

void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    red = Clamp(red, 0, 1);
    green = Clamp(green, 0, 1);
    blue = Clamp(blue, 0, 1);
    alpha = Clamp(alpha, 0, 1);
    if (context->blend_config.blend_r != red || context->blend_config.blend_g != green ||
        context->blend_config.blend_b != blue || context->blend_config.blend_a != alpha) {
        context->blend_config.blend_r = red;
        context->blend_config.blend_g = green;
        context->blend_config.blend_b = blue;
        context->blend_config.blend_a = alpha;
        context->blend_config.update_state |= 0b01;
    }
}

void glBlendEquation(GLenum mode) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (mode != GL_FUNC_ADD && mode != GL_FUNC_SUBTRACT && mode != GL_FUNC_REVERSE_SUBTRACT) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->blend_config.blend_equation_rgb != mode || context->blend_config.blend_equation_alpha != mode) {
        context->blend_config.blend_equation_rgb = mode;
        context->blend_config.blend_equation_alpha = mode;
        context->blend_config.update_state |= 0b10;
    }
}

void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if ((modeRGB != GL_FUNC_ADD && modeRGB != GL_FUNC_SUBTRACT && modeRGB != GL_FUNC_REVERSE_SUBTRACT) ||
        (modeAlpha != GL_FUNC_ADD && modeAlpha != GL_FUNC_SUBTRACT && modeAlpha != GL_FUNC_REVERSE_SUBTRACT)) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->blend_config.blend_equation_rgb != modeRGB || context->blend_config.blend_equation_alpha != modeAlpha) {
        context->blend_config.blend_equation_rgb = modeRGB;
        context->blend_config.blend_equation_alpha = modeAlpha;
        context->blend_config.update_state |= 0b10;
    }
}

bool CheckBlendFunc(GLenum sfactor, GLenum dfactor) {
    if (sfactor != GL_ZERO && sfactor != GL_ONE && (sfactor < GL_SRC_COLOR || sfactor > GL_SRC_ALPHA_SATURATE) &&
       (sfactor < GL_CONSTANT_COLOR || sfactor > GL_ONE_MINUS_CONSTANT_ALPHA)) {
        return false;
    }
    if (dfactor != GL_ZERO && dfactor != GL_ONE && (dfactor < GL_SRC_COLOR || dfactor > GL_ONE_MINUS_DST_COLOR) &&
       (dfactor < GL_CONSTANT_COLOR || dfactor > GL_ONE_MINUS_CONSTANT_ALPHA)) {
        return false;
    }
    return true;
}

void glBlendFunc(GLenum sfactor, GLenum dfactor) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (!CheckBlendFunc(sfactor, dfactor)) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->blend_config.blend_func_src_rgb != sfactor || context->blend_config.blend_func_src_alpha != sfactor) {
        context->blend_config.blend_func_src_rgb = sfactor;
        context->blend_config.blend_func_src_alpha = sfactor;
        context->blend_config.update_state |= 0b100;
    }
    if (context->blend_config.blend_func_dst_rgb != dfactor || context->blend_config.blend_func_dst_alpha != dfactor) {
        context->blend_config.blend_func_dst_rgb = dfactor;
        context->blend_config.blend_func_dst_alpha = dfactor;
        context->blend_config.update_state |= 0b100;
    }
}

void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (!CheckBlendFunc(srcRGB, dstRGB) || !CheckBlendFunc(srcAlpha, dstAlpha)) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->blend_config.blend_func_src_rgb != srcRGB || context->blend_config.blend_func_src_alpha != srcAlpha) {
        context->blend_config.blend_func_src_rgb = srcRGB;
        context->blend_config.blend_func_src_alpha = srcAlpha;
        context->blend_config.update_state |= 0b100;
    }
    if (context->blend_config.blend_func_dst_rgb != dstRGB || context->blend_config.blend_func_dst_alpha != dstAlpha) {
        context->blend_config.blend_func_dst_rgb = dstRGB;
        context->blend_config.blend_func_dst_alpha = dstAlpha;
        context->blend_config.update_state |= 0b100;
    }
}