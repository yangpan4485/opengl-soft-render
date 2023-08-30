#pragma once
#include <cstdint>
#include "gl_common.h"

GLenum glGetError();
void glGetIntegerv (GLenum pname, GLint *data);
void glGetFloatv (GLenum pname, GLfloat *data);
void glGetBooleanv (GLenum pname, GLboolean *data);
void glGetProgramiv (GLuint program, GLenum pname, GLint *params);
void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glGetShaderiv (GLuint shader, GLenum pname, GLint *params);
void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
const GLubyte* glGetString (GLenum name);