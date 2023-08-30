#pragma once
#include <cstdint>
#include "gl_common.h"

void glViewport(int x, int y, int width, int height);
void glDepthRangef(GLclampf nearVal, GLclampf farVal);