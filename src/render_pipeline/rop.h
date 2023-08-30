#pragma once

#include "gl_common.h"
#include "pipeline_struct.h"
#include "gl_config.h"

class Rop {
public:
    Rop();
    ~Rop();
    
    void SetBlendConfig(BlendConfig config);
    void SetDepthConfig(DepthConfig config);
    void SetStencilConfig(StencilConfig config);
    void SetScissorConfig(ScissorConfig config);
    void SetColorMask(ColorMask color_mask);

    void Clear(int mask);
    void ClearColor(float r, float g, float b, float a);
    void ClearDepth(uint32_t depth);
    void ClearStencil(uint32_t stencil);

    void Flush();
    void PushData(const PixelAttri& pixel);

private:
    bool ScissorTest(const PixelAttri& pixel);
    bool DepthTest(const PixelAttri& pixel);
    bool StencilTest(const PixelAttri& pixel);
    Color BlendTest(const PixelAttri& pixel);

    float CalcBlendFactorAlpha(int func, const Color& src, const Color& dst);
    std::vector<float> CalcBlendFactorRGB(int func, const Color& src, const Color& dst);
    uint8_t CalcBlendEquation(int func, uint8_t src, float src_factor, uint8_t dst, float dst_factor);
    void UpdateStencilBuffer(GLenum op_code, const PixelAttri& pixel);

    Color CalcColorMask(const Color& src, const Color& dst);

private:
    uint32_t clear_color_{};
    uint32_t clear_depth_{};
    uint32_t clear_stencil_{};

    BlendConfig blend_config_{};
    DepthConfig depth_config_{};
    StencilConfig stencil_config_{};
    ScissorConfig scissor_config_{};
    ColorMask color_mask_{};
};