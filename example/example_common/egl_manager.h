#pragma once
#include "common.h"
#include <EGL/egl.h>

class EGLManager{
public:
	EGLManager();
	~EGLManager();

    void SetDisplayID(DisplayID display_id);
	void SetWindow(WindowID window_id);
	bool Init();
	bool Destroy();
	void SwapBuffer();
	void MakeCurrent();

private:
	WindowID window_id_{};
	DisplayID display_id_{};
	EGLDisplay egl_display_{};
	EGLContext egl_context_{};
	EGLSurface egl_surface_{};
};