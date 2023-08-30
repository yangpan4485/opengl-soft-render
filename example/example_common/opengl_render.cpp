#include "opengl_render.h"

#include <iostream>
#include <iomanip>

#include <fstream>
#include <sstream>
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

OpenGLRender::OpenGLRender() {
    process_path_ = getcwd(NULL, 0);
	process_path_ = process_path_ + "/..";
}

OpenGLRender::~OpenGLRender() {
    Destroy();
}

bool OpenGLRender::Draw(uint32_t width, uint32_t height) {
    return DrawGraphics(width, height);
}

bool OpenGLRender::DrawGraphics(uint32_t width, uint32_t height) {
    return true;
}

bool OpenGLRender::Init() {
    return true;
}

void OpenGLRender::Destroy() {
}

std::string OpenGLRender::GetVertexShader() {
    return "";
}

std::string OpenGLRender::GetFragmentShader() {
    return "";
}