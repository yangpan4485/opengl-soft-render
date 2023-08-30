#include "gl_clear.h"

#include <cmath>
#include <algorithm>
#include <iostream>

#include "global_config.h"
#include "gl_utils.h"
#include "render_pipeline/global_instance.h"

void glClearColor(float r, float g, float b, float a) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    float clear_r = Clamp(r, 0, 1);
    float clear_g = Clamp(g, 0, 1);
    float clear_b = Clamp(b, 0, 1);
    float clear_a = Clamp(a, 0, 1);
    if (context->clear_config.clear_r != clear_r || context->clear_config.clear_g != clear_g ||
        context->clear_config.clear_b != clear_b || context->clear_config.clear_a != clear_a) {
        context->clear_config.clear_r = clear_r;
        context->clear_config.clear_g = clear_g;
        context->clear_config.clear_b = clear_b;
        context->clear_config.clear_a = clear_a;
        context->clear_config.update_state |= 0b01;
    }
}

void glClearDepthf(GLclampf depth) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    float clear_depth = Clamp(depth, 0, 1);
    if (context->clear_config.clear_depth != clear_depth) {
        context->clear_config.clear_depth = clear_depth;
        // std::cout << "glClearDepthf" << std::endl;
        context->clear_config.update_state |= 0b10;
    }
}

void glClearStencil(GLint s) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->clear_config.clear_stencil != s) {
        context->clear_config.clear_stencil = s;
        context->clear_config.update_state |= 0b100;
    }
}

void glClear(int mask) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    std::shared_ptr<Rop> rop_ins = GlobalInstance::GetInstance().GetRopInstance();
    if (mask & ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) {
         context->error_code = GL_INVALID_VALUE;
         return;
    }
    // if (mask)
    if (context->clear_config.update_state & 0b01) {
        rop_ins->ClearColor(context->clear_config.clear_r, context->clear_config.clear_g, context->clear_config.clear_b, context->clear_config.clear_a);
    }
    if (context->clear_config.update_state & 0b10) {
        uint32_t depth = context->clear_config.clear_depth * pow(2, 24);
        rop_ins->ClearDepth(depth);
    }
    if (context->clear_config.update_state & 0b100) {
        // clear stencil
    }
    rop_ins->Clear(mask);
    context->clear_config.update_state = 0;
}