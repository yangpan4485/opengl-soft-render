#include "x11_window.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

#ifdef FRAME_CALLBACK
#include "frame_buffer.h"
#endif

uint64_t X11GetTime() {
    auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

X11Window::X11Window() {
    XInitThreads();
}

X11Window::~X11Window() {
    Destroy();
}

void X11Window::Destroy() {
    if (!init_) {
        return;
    }
    if(window_created_) {
        XEvent ev;
        ev.xclient.type = ClientMessage;
        ev.xclient.window = window_;
        ev.xclient.message_type = 0;
        ev.xclient.format = 8;
        XSendEvent(x_display_, window_, False, 0L, &ev);
        XFlush(x_display_);
    }
    if(work_.joinable()) {
        work_.join();
    }
    if(x_display_) {
        // close x11 display
        XDestroyWindow(x_display_, window_);
        XCloseDisplay(x_display_);
        x_display_ = nullptr;
    }
    init_ = false;
#ifdef FRAME_CALLBACK
    if(image_data_) {
        delete[] image_data_;
    }
#endif
}

void X11Window::CreateX11Window() {
    work_ = std::thread([&](){
        x_display_ = XOpenDisplay(NULL);
        if(!x_display_) {
            std::cout << "open x11 display failed" << std::endl;
            return;
        }
        // get screen size
        int screen = DefaultScreen(x_display_);
        screen_width_ = DisplayWidth(x_display_, screen);
        screen_height_ = DisplayHeight(x_display_, screen);
        Window root = DefaultRootWindow(x_display_);
        XSetWindowAttributes swa;
        swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                     PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                     ExposureMask | FocusChangeMask | VisibilityChangeMask |
                     EnterWindowMask | LeaveWindowMask | PropertyChangeMask;
        window_width_ = 960;
        window_height_ = 540;
        delete_atom_ = XInternAtom(x_display_, "WM_DELETE_WINDOW", False);
        window_ = XCreateWindow(x_display_, root, 0, 0, window_width_, window_height_, 0, CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);
        XSetWMProtocols(x_display_, window_, &delete_atom_, 1);
        init_ = true;
        XWMHints hints;
	    hints.input = True;
	    hints.flags = InputHint;
	    XSetWMHints(x_display_, window_, &hints);

        // set window name
        XStoreName(x_display_ , window_ , "GL test");
        XMapWindow(x_display_, window_); // show window
        XEvent event;
        bool quit = false;
        // window_created_ = true;
        
#ifdef FRAME_CALLBACK
        visual_ = DefaultVisual(x_display_, 0);
        gc_ = XCreateGC(x_display_, window_, 0, NULL); 
        image_data_ = new char[window_width_ * window_height_ * 4];
#endif
        while(!quit) {
            XNextEvent(x_display_, &event);
            switch(event.type) {
            case Expose: {
                XWindowAttributes wndAttr;
                XGetWindowAttributes(x_display_, window_, &wndAttr);
                window_width_ = wndAttr.width;
                window_height_ = wndAttr.height;
                if (!window_created_) {
                    window_created_ = true;
                    cv_.notify_all();
                }
                if(callback_) {
                    callback_(kWindowSizeChange);
                }
                break;
            }
            case KeyPress: {
                KeySym event_key = XLookupKeysym(&(event.xkey), 0);
                std::string key_string = XKeysymToString(event_key);
                if("Escape" == key_string) {
                    quit = true;
                } else if("Left" == key_string) {
                    if(callback_) {
                        callback_(kWindowKeyPressLeft);
                    }
                } else if("Right" == key_string) {
                    if(callback_) {
                        callback_(kWindowKeyPressRight);
                    }
                }
                break;
            }
            case DestroyNotify: {
                quit = true;
                break;
            }
            case ClientMessage: {
                // std::cout << "event.xclient.message_type: " << event.xclient.message_type << std::endl;
                const Atom protocol = event.xclient.data.l[0];
                if (protocol == delete_atom_) {
                    quit = true;
                }
                if (event.xclient.message_type == 0) {
                    quit = true;
                }
#ifdef FRAME_CALLBACK
                if (event.xclient.message_type == 1) {
                    XPutImage(x_display_, window_, gc_, ximage_, 0, 0, 0, 0, window_width_,window_height_); 
                }
#endif
                break;
            }
            case MotionNotify: {
                // const int x = event.xmotion.x;
                // const int y = event.xmotion.y;
                break;
            }
            case ButtonPress: {
                break;
            }
            default:
                break;
            }
        }
        window_created_ = false;
        if(callback_) {
            callback_(kWindowClose);
        }
    });
    if(!window_created_) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock);
    }

#ifdef FRAME_CALLBACK
    FrameBuffer::GetInstance().SetFrameCallback([&](const std::vector<std::vector<uint32_t>>& data) {
        for (uint32_t j = 0; j < window_height_; ++j) {
            for (uint32_t i = 0; i < window_width_; ++i) {
                uint32_t pixel = data[i][j];
                uint32_t r = pixel & 0xff;
                uint32_t g = (pixel & 0xff00) >> 8;
                uint32_t b = (pixel & 0xff0000) >> 16;
                uint32_t a = (pixel & 0xff000000) >> 24;
                // int x = i + 1;
                int y = j + 1;
                y = window_height_ - y;
                int pos = y * window_width_ + i;
                pos = pos * 4;
                image_data_[pos] = b;
                image_data_[pos + 1] = g;
                image_data_[pos + 2] = r;
                image_data_[pos + 3] = a;
            }
        }
        ximage_ = XCreateImage(x_display_, visual_, 24, ZPixmap, 0, image_data_, window_width_, window_height_, 32, 0);
        XEvent ev;
        ev.xclient.type = ClientMessage;
        ev.xclient.window = window_;
        ev.xclient.message_type = 1;
        ev.xclient.format = 8;
        XSendEvent(x_display_, window_, False, 0L, &ev);
        XFlush(x_display_);
    });
#endif
}

Window X11Window::GetWindow() {
    return window_;
}

Display* X11Window::GetDisplay() {
    return x_display_;
}

uint32_t X11Window::GetWindowWidth() {
    return window_width_;
}

uint32_t X11Window::GetWindowHeight() {
    return window_height_;
}

void X11Window::SetWindowChangeCallback(WindowEventCallback callback) {
    callback_ = callback;
}

bool X11Window::IsWindowClose() {
    return !window_created_;
}
