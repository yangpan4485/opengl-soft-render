#pragma once
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <cstdint>
#include <thread>
#include <functional>

enum WindowEvent {
    kWindowClose = 0,
    kWindowSizeChange = 1,
    kWindowKeyPressUp = 2,
    kWindowKeyPressDown = 3,
    kWindowKeyPressLeft = 4,
    kWindowKeyPressRight = 5,
};

class X11Window {
public:
    using WindowEventCallback = std::function<void(WindowEvent event)>;
public:
    X11Window();
    ~X11Window();
    void Destroy();

    void CreateX11Window();
    void SetWindowChangeCallback(WindowEventCallback callback);

    uint32_t GetWindowWidth();
    uint32_t GetWindowHeight();
    Window GetWindow();
    Display* GetDisplay();
    bool IsWindowClose();

private:
    Display* x_display_{};
    bool init_{};
    uint32_t screen_width_{};
    uint32_t screen_height_{};
    uint32_t window_width_{};
    uint32_t window_height_{};
    Window window_{};
    std::thread work_{};
    bool running_ = false;

    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> window_created_{false};
    WindowEventCallback callback_{};
    Atom delete_atom_{};
#ifdef FRAME_CALLBACK
    Visual* visual_{};
    GC gc_{};
    char* image_data_{};
    XImage* ximage_{};
#endif
};