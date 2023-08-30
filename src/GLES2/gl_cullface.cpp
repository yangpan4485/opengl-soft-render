#include "gl_cullface.h"

#include "global_config.h"

void glCullFace(GLenum mode) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (mode < GL_FRONT || mode > GL_FRONT_AND_BACK) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->cullface_config.cullface_mode != mode) {
        context->cullface_config.cullface_mode = mode;
        context->cullface_config.update_state |= 0b10;
    }
}

void glFrontFace(GLenum mode) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (mode != GL_CW && mode != GL_CCW) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->cullface_config.front_face != mode) {
        context->cullface_config.front_face = mode;
        context->cullface_config.update_state |= 0b100;
    }
}