#pragma once
#include <cstdint>
#include "gl_common.h"

#include "gl_blend.h"
#include "gl_buffer.h"
#include "gl_clear.h"
#include "gl_cullface.h"
#include "gl_depth.h"
#include "gl_draw.h"
#include "gl_framebuffer.h"
#include "gl_get.h"
#include "gl_shader.h"
#include "gl_stencil.h"
#include "gl_texture.h"
#include "gl_uniform.h"
#include "gl_viewport.h"

void glFlush();
void glFinish();

void glLineWidth(float width);
void glEnable(GLenum cap);
void glDisable(GLenum cap);
GLboolean glIsEnabled (GLenum cap);
void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
void glHint(GLenum target, GLenum mode);
void glPolygonOffset(GLfloat factor, GLfloat units);
void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * data);
void glSampleCoverage(GLclampf value, GLboolean invert);