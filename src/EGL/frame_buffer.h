#pragma once
#include <cstdint>
#include <vector>

#include <fstream>
#include <functional>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <unistd.h>

class FrameBuffer {
public:
    using FrameCallback = std::function<void(const std::vector<std::vector<uint32_t>>& data)>;
public:
    static FrameBuffer& GetInstance();
    void SetFrameSize(uint16_t width, uint32_t height);

    void ClearColor(uint32_t color);
    void ClearDepth(uint32_t depth);
    void ClearStencil(uint32_t stencil);

    void SetDepth(uint16_t x, uint16_t y, uint32_t depth);
    void SetStencil(uint16_t x, uint16_t y, uint32_t stencil);
    void SetPixel(uint16_t x, uint16_t y, uint32_t pixel);

    uint32_t GetDepth(uint16_t x, uint16_t y);
    uint32_t GetStencil(uint16_t x, uint16_t y);
    uint32_t GetPixel(uint16_t x, uint16_t y);

    void Flush();
    void SetFrameCallback(FrameCallback callback);

private:
    FrameBuffer();
    ~FrameBuffer();

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer operator=(const FrameBuffer&) = delete;

private:
    uint32_t depth_{};
    uint32_t stencil_{};

    std::vector<std::vector<uint32_t>> depth_buffer_{};
    std::vector<std::vector<uint32_t>> stencil_buffer_{};
    std::vector<std::vector<uint32_t>> pixel_buffer_{};

    int frame_width_{};
    int frame_height_{};

    // std::ofstream fout_{};
    FrameCallback frame_callback_{};

};