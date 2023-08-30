#pragma once

#include <cstdint>
#include "gl_common.h"

void glClearColor(float r, float g, float b, float a);
void glClearDepthf(GLclampf depth);
void glClearStencil(GLint s);
void glClear(int mask);