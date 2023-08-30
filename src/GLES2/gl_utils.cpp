#include "gl_utils.h"

float Clamp(float val, float min, float max) {
    val = val > min ? val : min;
    val = val < max ? val : max;
    return val;
}