#pragma once
#include <cstdint>
#include "gl_common.h"

void glGenFramebuffers(GLsizei n, GLuint* framebuffers);
void glBindFramebuffer(GLenum target, GLuint framebuffer);
void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers);
void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers);
void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers);
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params);
void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params);
GLenum glCheckFramebufferStatus(GLenum target);
GLboolean glIsRenderbuffer(GLuint renderbuffer);
GLboolean glIsFramebuffer(GLuint framebuffer);