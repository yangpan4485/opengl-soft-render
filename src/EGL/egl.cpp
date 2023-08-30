#include "egl.h"

#include <iostream>

#include "gl2.h"
#include "frame_buffer.h"

Display* display;

EGLDisplay eglGetDisplay(EGLNativeDisplayType display_id) {
    display = (Display*)display_id;
    return nullptr;
}

EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list) {
    int window_id = win;
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window_id, &attrs);
    // std::cout << "width: " << attrs.width << std::endl;
    // std::cout << "height: " << attrs.height << std::endl;
    FrameBuffer::GetInstance().SetFrameSize(attrs.width, attrs.height);
    return nullptr;
}

EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list) {
    return nullptr;
}

bool eglInitialize(EGLDisplay dpy, EGLint* major, EGLint* minor) {
    return true;
}

bool eglChooseConfig(EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config) {
    return true;
}

bool eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) {
    return true;
}

bool eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    glFlush();
    return true;
}

bool eglDestroyContext (EGLDisplay dpy, EGLContext ctx) {
    return true;
}

bool eglDestroySurface (EGLDisplay dpy, EGLSurface surface) {
    return true;
}

bool eglTerminate (EGLDisplay dpy) {
    return true;
}