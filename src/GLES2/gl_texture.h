#pragma once
#include <cstdint>
#include "gl_common.h"

void glGenTextures(GLsizei n, GLuint * textures);
void glBindTexture(GLenum target, GLuint texture);
void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * data);
void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
void glTexParameteri(GLenum target, GLenum pname, GLint param);
void glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params);
void glTexParameteriv(GLenum target, GLenum pname, const GLint * params);
void glActiveTexture(GLenum texture);
void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data);
void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,  GLint yoffset, GLsizei width, GLsizei height,  GLenum format, GLsizei imageSize,  const void * data);
void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,  GLint yoffset, GLint x, GLint y, GLsizei width,  GLsizei height);
void glGenerateMipmap(GLenum target);
void glDeleteTextures(GLsizei n, const GLuint* textures);
void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params);
void glGetTexParameteriv(GLenum target, GLenum pname, GLint * params);
void glPixelStorei(GLenum pname, GLint param);
GLboolean glIsTexture(GLuint texture);