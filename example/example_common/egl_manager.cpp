#include "egl_manager.h"
#include <iostream>

EGLManager::EGLManager() = default;

EGLManager::~EGLManager() = default;

void EGLManager::SetDisplayID(DisplayID display_id) {
	display_id_ = display_id;
}

void EGLManager::SetWindow(WindowID window_id) {
	window_id_ = window_id;
}

bool EGLManager::Init() {
#ifdef WIN32
	egl_display_ = eglGetDisplay(GetDC(window_id_));
#else
	egl_display_ = eglGetDisplay((EGLNativeDisplayType)display_id_);
#endif
	EGLint MajorVersion;
	EGLint MinorVersion;
	if (!eglInitialize(egl_display_, &MajorVersion, &MinorVersion)) {
		std::cout << "init egl failed" << std::endl;
		return false;
	}
	const EGLint attr[] = {
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_STENCIL_SIZE, 8,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE
	};

	EGLConfig ecfg;
	EGLint num_config;
	if (!eglChooseConfig(egl_display_, attr, &ecfg, 1, &num_config)) {
		std::cout << "egl choose config failed" << std::endl;
		return false;
	}
	egl_surface_ = eglCreateWindowSurface(egl_display_, ecfg, window_id_, NULL);
	EGLint ctxattr[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	egl_context_ = eglCreateContext(egl_display_, ecfg, EGL_NO_CONTEXT, ctxattr);
	eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_);
	return true;
}

bool EGLManager::Destroy() {
	eglMakeCurrent(egl_display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(egl_display_, egl_context_);
	eglDestroySurface(egl_display_, egl_surface_);
	eglTerminate(egl_display_);
	return true;
}

void EGLManager::SwapBuffer() {
	eglSwapBuffers(egl_display_, egl_surface_);
}

void EGLManager::MakeCurrent() {
	eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_);
}