#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <chrono>

#include "x11_window.h"
#include "opengl_render.h"
#include "egl_manager.h"

#include "point/opengl_render_point.h"
#include "point/opengl_render_point_size.h"
#include "line/opengl_render_line.h"
#include "line/opengl_render_line_strip.h"
#include "line/opengl_render_line_loop.h"
#include "line/opengl_render_line_width.h"
#include "triangle/opengl_render_triangle_strip.h"
#include "triangle/opengl_render_triangle_fan.h"
#include "triangle/opengl_render_triangle.h"
#include "triangle/opengl_render_multi_primitive.h"
#include "texture/texture_common.h"
#include "texture/opengl_render_texture.h"
#include "texture/opengl_render_texture_wrap_repeat.h"
#include "texture/opengl_render_texture_mirror_repeat.h"
#include "texture/opengl_render_texture_wrap_edge.h"
#include "texture/opengl_render_texture_filter_linear.h"
#include "texture/opengl_render_texture_copy.h"
#include "texture/opengl_render_texture_copy_sub.h"
#include "texture/opengl_render_read_pixel.h"
#include "texture/opengl_render_texture_overlay.h"
#include "texture/opengl_render_texture_sub.h"
#include "texture/opengl_render_buffer_sub.h"
#include "texture/opengl_render_scissor.h"
#include "texture/opengl_render_texture_advanced.h"
#include "texture/opengl_render_texture_yuv.h"
#include "texture/opengl_render_texture_compress.h"
#include "texture/opengl_render_texture_compress_sub.h"
#include "cullface/opengl_render_cullface.h"
#include "depth_test/opengl_render_depth_test.h"
#include "blend_test/opengl_render_blend_test.h"
#include "stencil_test/opengl_render_stencil_test.h"
#include "fbo/opengl_render_fbo.h"
#include "cube_map/opengl_render_cube_map.h"
#include "utils.h"

using namespace std::chrono;
uint64_t GetTimestamp() {
	system_clock::duration d = system_clock::now().time_since_epoch();
	milliseconds mil = duration_cast<milliseconds>(d);
	return mil.count();
}

X11Window custom_window;
std::shared_ptr<EGLManager> egl_manager;
std::mutex mtx;
std::condition_variable cv;
bool window_close = false;

void Show(std::shared_ptr<OpenGLRender> opengl_render) {
	auto start = GetTimeStamp();
    if (window_close) {
		return;
	}
	uint32_t width = custom_window.GetWindowWidth();
	uint32_t height = custom_window.GetWindowHeight();
    if(!opengl_render->Init()) {
        std::cout << "Init Failed" << std::endl;
        return;
    }
    opengl_render->Draw(width, height);
	egl_manager->SwapBuffer();
	auto end = GetTimeStamp();
	std::cout << "end - start: " << (end - start) << std::endl;
    std::unique_lock<std::mutex> lock(mtx);
	cv.wait_for(lock, std::chrono::seconds(13));
    opengl_render->Destroy();
}

void ShowPoint() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderPoint());
	Show(opengl_render);
}

void ShowPointSize() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderPointSize());
	Show(opengl_render);
}

void ShowLine() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderLine());
	Show(opengl_render);
}

void ShowLineLoop() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderLineLoop());
	Show(opengl_render);
}

void ShowLineStrip() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderLineStrip());
	Show(opengl_render);
}

void ShowLineWidth() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderLineWidth());
	Show(opengl_render);
}

void ShowTriangle() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTriangle());
	Show(opengl_render);
}

void ShowTriangleFan() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTriangleFan());
	Show(opengl_render);
}

void ShowTriangleStrip() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTriangleStrip());
	Show(opengl_render);
}

void ShowMultiPrimitive() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderMultiPrimitive());
	Show(opengl_render);
}

void ShowTexture() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTexture());
	Show(opengl_render);
}

void ShowTextureWrapRepeat() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureWrapRepeat());
	Show(opengl_render);
}

void ShowTextureMirrorRepeat() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureMirrorRepeat());
	Show(opengl_render);
}

void ShowTextureWrapEdge() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureWrapEdge());
	Show(opengl_render);
}

void ShowTextureFilterLinear() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureFilterLinear());
	Show(opengl_render);
}

void ShowTextureCopy() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureCopy());
	Show(opengl_render);
}

void ShowTextureCopySub() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureCopySub());
	Show(opengl_render);
}

void ShowTextureReadPixel() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderReadPixel());
	Show(opengl_render);
}

void ShowTextureOverlay() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureOverlay());
	Show(opengl_render);
}

void ShowTextureSub() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureSub());
	Show(opengl_render);
}

void ShowBufferSub() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderBufferSub());
	Show(opengl_render);
}

void ShowScissor() {
    std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderScissor());
	Show(opengl_render);
}

void ShowTextureAdvanced() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureAdvanced());
	Show(opengl_render);
}

void ShowTextureYUV() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureYUV());
	if (window_close) {
		return;
	}
	uint32_t width = custom_window.GetWindowWidth();
	uint32_t height = custom_window.GetWindowHeight();
    if(!opengl_render->Init()) {
        std::cout << "Init Failed" << std::endl;
        return;
    }
	for (int i = 0; i < 10; ++i) {
		if (window_close) {
		    return;
	    }
		opengl_render->Draw(width, height);
		egl_manager->SwapBuffer();
	}
    opengl_render->Destroy();
}

void ShowTextureCompress() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureCompress());
	Show(opengl_render);
}

void ShowTextureCompressSub() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderTextureCompressSub());
	Show(opengl_render);
}

void ShowCullface() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderCullface());
	if (window_close) {
		return;
	}
	uint32_t width = custom_window.GetWindowWidth();
	uint32_t height = custom_window.GetWindowHeight();
    if(!opengl_render->Init()) {
        std::cout << "Init Failed" << std::endl;
        return;
    }
	for (int i = 0; i < 100; ++i) {
		if (window_close) {
		    return;
	    }
		opengl_render->Draw(width, height);
		egl_manager->SwapBuffer();
	}
	std::unique_lock<std::mutex> lock(mtx);
    cv.wait_for(lock, std::chrono::seconds(13));
    opengl_render->Destroy();
}

void ShowDepthTest() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderDepthTest());
	if (window_close) {
		return;
	}
	uint32_t width = custom_window.GetWindowWidth();
	uint32_t height = custom_window.GetWindowHeight();
    if(!opengl_render->Init()) {
        std::cout << "Init Failed" << std::endl;
        return;
    }
	for (int i = 0; i < 100; ++i) {
		if (window_close) {
		    return;
	    }
		opengl_render->Draw(width, height);
		egl_manager->SwapBuffer();
	}
	std::unique_lock<std::mutex> lock(mtx);
    cv.wait_for(lock, std::chrono::seconds(13));
    opengl_render->Destroy();
}

void ShowBlendTest() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderBlendTest());
	Show(opengl_render);
}

void ShowStencilTest() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderStencilTest());
	Show(opengl_render);
}

void ShowFBO() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderFrameBuffer());
	Show(opengl_render);
}

void ShowCubeMap() {
	std::shared_ptr<OpenGLRender> opengl_render(new OpenGLRenderCubeMap());
	Show(opengl_render);
}

int main(void) {
    custom_window.CreateX11Window();
    egl_manager.reset((new EGLManager()));
	egl_manager->SetWindow(custom_window.GetWindow());
	egl_manager->SetDisplayID(custom_window.GetDisplay());
    custom_window.SetWindowChangeCallback([&](WindowEvent event) {
		if (event == kWindowKeyPressRight) {
			cv.notify_all();
		}
		if (event == kWindowClose) {
			window_close = true;
			cv.notify_all();
		}
	});
	egl_manager->MakeCurrent();
	egl_manager->Init();
    // ShowPoint();
    // ShowPointSize();
    // ShowLine();
    // ShowLineLoop();
    // ShowLineStrip();
    // ShowLineWidth();
    // ShowTriangle();
    // ShowTriangleFan();
    // ShowTriangleStrip();
	// ShowMultiPrimitive();
	// ShowTexture();
	// ShowTextureWrapRepeat();
	// ShowTextureMirrorRepeat();
	// ShowTextureWrapEdge();
	// ShowTextureFilterLinear();
	// ShowTextureCopy();
	// ShowTextureCopySub();
	// ShowTextureReadPixel();
	// ShowTextureOverlay();
	// ShowTextureSub();
	// ShowBufferSub();
	// ShowScissor();
	// ShowTextureAdvanced();
	// ShowTextureYUV();
	// ShowTextureCompress();
	// ShowTextureCompressSub();
	// ShowCullface();
	// ShowDepthTest();
	// ShowBlendTest();
	// ShowStencilTest();
	ShowFBO();
	ShowCubeMap();
    custom_window.Destroy();
	egl_manager->Destroy();
    return 0;
}