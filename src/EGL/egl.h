#pragma once
#include <cstdint>

typedef void* EGLDisplay;
typedef void* EGLSurface;
typedef void* EGLNativeDisplayType;
typedef void* EGLConfig;
typedef void* EGLContext;
typedef int EGLNativeWindowType;
typedef int32_t EGLint;

#define EGL_CAST(type, value) (static_cast<type>(value))
#define EGL_NO_CONTEXT                    EGL_CAST(EGLContext,0)
#define EGL_NO_DISPLAY                    EGL_CAST(EGLDisplay,0)
#define EGL_NO_SURFACE                    EGL_CAST(EGLSurface,0)

#define EGL_CONTEXT_CLIENT_VERSION        0x3098
#define EGL_RENDERABLE_TYPE               0x3040
#define EGL_NONE                          0x3038
#define EGL_STENCIL_SIZE                  0x3026
#define EGL_DEPTH_SIZE                    0x3025
#define EGL_RED_SIZE                      0x3024
#define EGL_GREEN_SIZE                    0x3023
#define EGL_BLUE_SIZE                     0x3022
#define EGL_ALPHA_SIZE                    0x3021
#define EGL_OPENGL_ES2_BIT                0x0004

EGLDisplay eglGetDisplay(EGLNativeDisplayType display_id);
EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
bool eglInitialize(EGLDisplay dpy, EGLint* major, EGLint* minor);
bool eglChooseConfig(EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config);
bool eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
bool eglSwapBuffers(EGLDisplay dpy, EGLSurface surface);
bool eglDestroyContext (EGLDisplay dpy, EGLContext ctx);
bool eglDestroySurface (EGLDisplay dpy, EGLSurface surface);
bool eglTerminate (EGLDisplay dpy);