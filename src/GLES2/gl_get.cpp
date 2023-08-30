#include "gl_get.h"

#include "global_config.h"

GLenum glGetError() {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int error = context->error_code;
    context->error_code = GL_NO_ERROR;
    return error;
}

void glGetIntegerv (GLenum pname, GLint *data) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (pname == GL_SHADER_COMPILER) {
        *data = 1;
    }
    if (pname == GL_VIEWPORT) {
        data[0] = context->viewport_config.x;
        data[1] = context->viewport_config.y;
        data[2] = context->viewport_config.width;
        data[3] = context->viewport_config.height;
    }
    if (pname == GL_MAX_VIEWPORT_DIMS) {
        data[0] = context->viewport_config.max_width;
        data[1] = context->viewport_config.max_height;
    }
}

void glGetFloatv (GLenum pname, GLfloat *data) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (pname == GL_COLOR_CLEAR_VALUE) {
        data[0] = context->clear_config.clear_r;
        data[1] = context->clear_config.clear_g;
        data[2] = context->clear_config.clear_b;
        data[3] = context->clear_config.clear_a;
    }
}

void glGetBooleanv (GLenum pname, GLboolean *data) {

}

void glGetProgramiv (GLuint program, GLenum pname, GLint *params) {
    if (pname == GL_LINK_STATUS) {
        *params = 1;
    }
}

void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {}

void glGetShaderiv (GLuint shader, GLenum pname, GLint *params) {
    if (pname == GL_COMPILE_STATUS) {
        *params = 1;
    }
}

void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {}

const GLubyte* glGetString (GLenum name) {
    return nullptr;
}