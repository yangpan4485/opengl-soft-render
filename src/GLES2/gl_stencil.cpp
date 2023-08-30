#include "gl_stencil.h"

#include "global_config.h"

void glStencilFuncFront(GLenum func, GLint ref, GLuint mask) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (func < GL_NEVER || func > GL_ALWAYS) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->stencil_config.front_stencil_func != func) {
        context->stencil_config.front_stencil_func = func;
        context->stencil_config.update_state |= 0b01;
    }
    if (context->stencil_config.front_stencil_func_ref != ref) {
        context->stencil_config.front_stencil_func_ref = ref;
        context->stencil_config.update_state |= 0b01;
    }
    if (context->stencil_config.front_stencil_func_mask != mask) {
        context->stencil_config.front_stencil_func_mask = mask;
        context->stencil_config.update_state |= 0b01;
    }
}

void glStencilFuncBack(GLenum func, GLint ref, GLuint mask) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (func < GL_NEVER || func > GL_ALWAYS) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->stencil_config.back_stencil_func != func) {
        context->stencil_config.back_stencil_func = func;
        context->stencil_config.update_state |= 0b10;
    }
    if (context->stencil_config.back_stencil_func_ref != ref) {
        context->stencil_config.back_stencil_func_ref = ref;
        context->stencil_config.update_state |= 0b10;
    }
    if (context->stencil_config.back_stencil_func_mask != mask) {
        context->stencil_config.back_stencil_func_mask = mask;
        context->stencil_config.update_state |= 0b10;
    }
}

void glStencilFunc(GLenum func, GLint ref, GLuint mask) {
    glStencilFuncFront(func, ref, mask);
    glStencilFuncBack(func, ref, mask);
}

void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (face < GL_FRONT || face > GL_FRONT_AND_BACK) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (face == GL_FRONT) {
        glStencilFuncFront(func, ref, mask);
    } else if (face == GL_BACK) {
        glStencilFuncBack(func, ref, mask);
    } else {
        glStencilFunc(func, ref, mask);
    }
}

void glStencilMaskFront(GLuint mask) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->stencil_config.front_mask != mask) {
        context->stencil_config.front_mask = mask;
        context->stencil_config.update_state |= 0b100;
    }
}

void glStencilMaskBack(GLuint mask) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->stencil_config.back_mask != mask) {
        context->stencil_config.back_mask = mask;
        context->stencil_config.update_state |= 0b100;
    }
}

void glStencilMask(GLuint mask) {
    glStencilMaskFront(mask);
    glStencilMaskBack(mask);
}

void glStencilMaskSeparate(GLenum face, GLuint mask) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (face < GL_FRONT || face > GL_FRONT_AND_BACK) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (face == GL_FRONT) {
        glStencilMaskFront(mask);
    } else if (face == GL_BACK) {
        glStencilMaskBack(mask);
    } else {
        glStencilMask(mask);
    }
}

void glStencilOpFront(GLenum sfail, GLenum dpfail, GLenum dppass) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if ((sfail != 0 && (sfail < GL_KEEP || sfail > GL_DECR_WRAP)) ||
        (dpfail != 0 && (dpfail < GL_KEEP || dpfail > GL_DECR_WRAP)) ||
        (dppass != 0 && (dppass < GL_KEEP || dppass > GL_DECR_WRAP))) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->stencil_config.front_sfail != sfail || context->stencil_config.front_dpfail != dpfail || 
        context->stencil_config.front_dppass != dppass) {
        context->stencil_config.front_sfail = sfail;
        context->stencil_config.front_dpfail = dpfail;
        context->stencil_config.front_dppass = dppass;
        context->stencil_config.update_state |= 0b1000;
    }
}

void glStencilOpBack(GLenum sfail, GLenum dpfail, GLenum dppass) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if ((sfail != 0 && (sfail < GL_KEEP || sfail > GL_DECR_WRAP)) ||
        (dpfail != 0 && (dpfail < GL_KEEP || dpfail > GL_DECR_WRAP)) ||
        (dppass != 0 && (dppass < GL_KEEP || dppass > GL_DECR_WRAP))) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->stencil_config.back_sfail != sfail || context->stencil_config.back_dpfail != dpfail || 
        context->stencil_config.back_dppass != dppass) {
        context->stencil_config.back_sfail = sfail;
        context->stencil_config.back_dpfail = dpfail;
        context->stencil_config.back_dppass = dppass;
        context->stencil_config.update_state |= 0b10000;
    }
}

void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {
    glStencilOpFront(sfail, dpfail, dppass);
    glStencilOpBack(sfail, dpfail, dppass);
}

void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (face < GL_FRONT || face > GL_FRONT_AND_BACK) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (face == GL_FRONT) {
        glStencilOpFront(sfail, dpfail, dppass);
    } else if (face == GL_BACK) {
        glStencilOpBack(sfail, dpfail, dppass);
    } else {
        glStencilOp(sfail, dpfail, dppass);
    }
}