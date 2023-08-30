#pragma once
#include <cstdint>
#include "gl_common.h"

GLuint glCreateShader(GLenum type);
void glDeleteShader (GLuint shader);
void glShaderSource(GLuint shader, int count, const char* const* string, const int* length);
void glCompileShader(GLuint shader);
void glReleaseShaderCompiler();

GLuint glCreateProgram();
void glDeleteProgram (GLuint program);
void glAttachShader(GLuint program, GLuint shader);
void glDetachShader (GLuint program, GLuint shader);
void glLinkProgram(GLuint program);
void glUseProgram(GLuint program);

void glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
void glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
void glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
GLboolean glIsProgram (GLuint program);
GLboolean glIsShader (GLuint shader);
void glShaderBinary (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
void glValidateProgram (GLuint program);