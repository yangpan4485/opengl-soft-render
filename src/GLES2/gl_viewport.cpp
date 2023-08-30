#include "gl_viewport.h"
#include "gl_utils.h"

#include "global_config.h"

void glViewport(int x, int y, int width, int height) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (width < 0 || height < 0) {
        context->error_code = GL_INVALID_VALUE;
        return ;
    }
    width = Clamp(width, 0, context->viewport_config.max_width);
    height = Clamp(height, 0, context->viewport_config.max_height);
    if (context->viewport_config.x != x || context->viewport_config.y != y ||
        context->viewport_config.width != width || context->viewport_config.height != height) {
        context->viewport_config.x = x;
        context->viewport_config.y = y;
        context->viewport_config.width = width;
        context->viewport_config.height = height;
        context->viewport_config.update_state |= 0b01;
    } 
    
}

void glDepthRangef(GLclampf nearVal, GLclampf farVal) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    nearVal = Clamp(nearVal, 0, 1);
    farVal = Clamp(farVal, 0, 1);
    if (context->viewport_config.near != nearVal || context->viewport_config.far != farVal) {
        context->viewport_config.near = nearVal;
        context->viewport_config.far = farVal;
        context->viewport_config.update_state |= 0b10;
    }
}