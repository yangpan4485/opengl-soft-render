#pragma once

#include <cstdint>

uint8_t etc2_clamp(int color);
uint8_t etc1_base_color_ind_hi(uint8_t in);
uint8_t etc1_base_color_ind_lo(uint8_t in);
uint8_t etc1_base_color_diff_hi(uint8_t in);
uint8_t etc1_base_color_diff_lo(uint8_t in);
uint8_t etc2_base_color1_t_mode(const uint8_t* in, int index);
uint8_t etc2_base_color2_t_mode(const uint8_t* in, int index);
uint8_t etc2_base_color1_h_mode(const uint8_t* in, int index);
uint8_t etc2_base_color2_h_mode(const uint8_t* in, int index);
uint8_t etc2_base_color_o_planar(const uint8_t* in, int index);
uint8_t etc2_base_color_h_planar(const uint8_t* in, int index);
uint8_t etc2_base_color_v_planar(const uint8_t* in, int index);