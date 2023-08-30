#include "gl_depth.h"

#include "global_config.h"

void glDepthFunc(GLenum func) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (func < GL_NEVER || func > GL_ALWAYS) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->depth_config.func != func) {
        context->depth_config.func = func;
        context->depth_config.update_state |= 0b01;
    }
}

void glDepthMask(GLboolean flag) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->depth_config.mask != flag) {
        context->depth_config.mask = flag;
        context->depth_config.update_state |= 0b10;
    }
}
