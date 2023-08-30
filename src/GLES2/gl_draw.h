#pragma once

#include "gl_common.h"

void glDrawArrays(int mode, int first, int count);
void glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices);
