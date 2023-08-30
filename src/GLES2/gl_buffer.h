#pragma once

#include <cstdint>
#include "gl_common.h"

void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name);
void glGenBuffers(int n, GLuint* buffers);
void glDeleteBuffers (GLsizei n, const GLuint *buffers);
void glBindBuffer(int target, GLuint buffer);
void glBufferData(int target, int size, const void* data, int usage);
void glVertexAttribPointer(GLuint index, int size, int type, bool normalized, int stride, const void* pointer);
void glEnableVertexAttribArray(GLuint index);
void glDisableVertexAttribArray(GLuint index);
void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params);
GLint glGetAttribLocation (GLuint program, const GLchar *name);
void glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params);
void glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params);
void glGetVertexAttribPointerv (GLuint index, GLenum pname, void **pointer);
GLboolean glIsBuffer (GLuint buffer);
void glVertexAttrib1f (GLuint index, GLfloat x);
void glVertexAttrib1fv (GLuint index, const GLfloat *v);
void glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y);
void glVertexAttrib2fv (GLuint index, const GLfloat *v);
void glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z);
void glVertexAttrib3fv (GLuint index, const GLfloat *v);
void glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glVertexAttrib4fv (GLuint index, const GLfloat *v);