#pragma once
#include <cstdint>
#include "gl_common.h"

struct ViewportConfig {
    int x{};
    int y{};
    int width{960};
    int height{540};
    float near{0.0};
    float far{1.0};
    int max_width{4096};
    int max_height{4096};
    int update_state{};
};

struct CullfaceConfig {
    bool enable{};
    int front_face{GL_CCW};
    int cullface_mode{GL_BACK};
    int update_state{};
};

struct RasterConfig {
    int linewidth{};
    int update_state{};
};

struct ClearConfig {
    float clear_r{};
    float clear_g{};
    float clear_b{};
    float clear_a{};

    int clear_stencil{};
    float clear_depth{1.0};
    int update_state{};
};

struct StencilConfig {
    int front_stencil_func{GL_ALWAYS};
    int front_stencil_func_ref{};
    int front_stencil_func_mask{0xff};
    int back_stencil_func{GL_ALWAYS};
    int back_stencil_func_ref{};
    int back_stencil_func_mask{0xff};
    int front_mask{0xff};
    int back_mask{0xff};
    int front_sfail{GL_KEEP};
    int front_dpfail{GL_KEEP};
    int front_dppass{GL_KEEP};
    int back_sfail{GL_KEEP};
    int back_dpfail{GL_KEEP};
    int back_dppass{GL_KEEP};
    bool enable_stencil{false};
    int update_state{};
};

struct DepthConfig {
    int func{GL_LESS};
    bool mask{true};
    bool enable_depth{false};
    int update_state{};
};

struct BlendConfig {
    float blend_r{};
    float blend_g{};
    float blend_b{};
    float blend_a{};
    int blend_equation_rgb{GL_FUNC_ADD};
    int blend_equation_alpha{GL_FUNC_ADD};
    int blend_func_src_rgb{GL_ONE};
    int blend_func_dst_rgb{GL_ZERO};
    int blend_func_src_alpha{GL_ONE};
    int blend_func_dst_alpha{GL_ZERO};
    bool enable_blend{false};;
    int update_state{};
};

struct ScissorConfig {
    int scissor_x{};
    int scissor_y{};
    int scissor_width{};
    int scissor_height{};
    bool enable_scissor{false};
    int update_state{};
};

struct ColorMask {
    bool red{true};
    bool green{true};
    bool blue{true};
    bool alpha{true};
};