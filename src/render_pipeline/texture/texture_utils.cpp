#include "texture_utils.h"

#include "gl_utils.h"

uint8_t etc2_clamp(int color) {
    /* CLAMP(color, 0, 255) */
    return (uint8_t)Clamp(color, 0, 255);
}

uint8_t etc1_base_color_ind_hi(uint8_t in) {
    return (in & 0xf0) | ((in & 0xf0) >> 4);
}

uint8_t etc1_base_color_ind_lo(uint8_t in) {
    return ((in & 0xf) << 4) | (in & 0xf);
}

uint8_t etc1_base_color_diff_hi(uint8_t in) {
    return (in & 0xf8) | (in >> 5);
}

uint8_t etc1_base_color_diff_lo(uint8_t in) {
    static const int lookup[8] = { 0, 1, 2, 3, -4, -3, -2, -1 };
    in = (in >> 3) + lookup[in & 0x7];
    return (in << 3) | (in >> 2);
}

uint8_t etc2_base_color1_t_mode(const uint8_t* in, int index) {
    uint8_t R1a = 0, x = 0;
    /* base col 1 = extend_4to8bits( (R1a << 2) | R1b, G1, B1) */
    switch (index) {
    case 0:
        R1a = (in[0] >> 3) & 0x3;
        x = ((R1a << 2) | (in[0] & 0x3));
        break;
    case 1:
        x = ((in[1] >> 4) & 0xf);
        break;
    case 2:
        x = (in[1] & 0xf);
        break;
    default:
        /* invalid index */
        break;
    }
    return ((x << 4) | (x & 0xf));
}

uint8_t etc2_base_color2_t_mode(const uint8_t* in, int index) {
    uint8_t x = 0;
    /*extend 4to8bits(R2, G2, B2)*/
    switch (index) {
    case 0:
        x = ((in[2] >> 4) & 0xf);
        break;
    case 1:
        x = (in[2] & 0xf);
        break;
    case 2:
        x = ((in[3] >> 4) & 0xf);
        break;
    default:
        /* invalid index */
        break;
    }
    return ((x << 4) | (x & 0xf));
}

uint8_t etc2_base_color1_h_mode(const uint8_t* in, int index) {
    uint8_t x = 0;
    /* base col 1 = extend 4to8bits(R1, (G1a << 1) | G1b, (B1a << 3) | B1b) */
    switch (index) {
    case 0:
        x = ((in[0] >> 3) & 0xf);
        break;
    case 1:
        x = (((in[0] & 0x7) << 1) | ((in[1] >> 4) & 0x1));
        break;
    case 2:
        x = ((in[1] & 0x8) |
            (((in[1] & 0x3) << 1) | ((in[2] >> 7) & 0x1)));
        break;
    default:
        /* invalid index */
        break;
    }
    return ((x << 4) | (x & 0xf));
}

uint8_t etc2_base_color2_h_mode(const uint8_t* in, int index) {
    uint8_t x = 0;
    /* base col 2 = extend 4to8bits(R2, G2, B2) */
    switch (index) {
    case 0:
        x = ((in[2] >> 3) & 0xf);
        break;
    case 1:
        x = (((in[2] & 0x7) << 1) | ((in[3] >> 7) & 0x1));
        break;
    case 2:
        x = ((in[3] >> 3) & 0xf);
        break;
    default:
        /* invalid index */
        break;
    }
    return ((x << 4) | (x & 0xf));
}

uint8_t etc2_base_color_o_planar(const uint8_t* in, int index) {
    int tmp;
    switch (index) {
    case 0:
        tmp = ((in[0] >> 1) & 0x3f); /* RO */
        return ((tmp << 2) | (tmp >> 4));
    case 1:
        tmp = (((in[0] & 0x1) << 6) | ((in[1] >> 1) & 0x3f));
        return ((tmp << 1) | (tmp >> 6));
    case 2:
        tmp = (((in[1] & 0x1) << 5) | (in[2] & 0x18) | (((in[2] & 0x3) << 1) | ((in[3] >> 7) & 0x1)));
        return ((tmp << 2) | (tmp >> 4));
    default:
        /* invalid index */
        return 0;
    }
}

uint8_t etc2_base_color_h_planar(const uint8_t* in, int index) {
    int tmp;
    switch (index) {
    case 0:
        tmp = (((in[3] & 0x7c) >> 1) | (in[3] & 0x1));
        return ((tmp << 2) | (tmp >> 4));
    case 1:
        tmp = (in[4] >> 1) & 0x7f; /* GH */
        return ((tmp << 1) | (tmp >> 6));
    case 2:
        tmp = (((in[4] & 0x1) << 5) | ((in[5] >> 3) & 0x1f));
        return ((tmp << 2) | (tmp >> 4));
    default:
        /* invalid index */
        return 0;
    }
}

uint8_t etc2_base_color_v_planar(const uint8_t* in, int index) {
    int tmp;
    switch (index) {
    case 0:
        tmp = (((in[5] & 0x7) << 0x3) | ((in[6] >> 5) & 0x7));
        return ((tmp << 2) | (tmp >> 4));
    case 1:
        tmp = (((in[6] & 0x1f) << 2) | ((in[7] >> 6) & 0x3));
        return ((tmp << 1) | (tmp >> 6));
    case 2:
        tmp = in[7] & 0x3f; /* BV */
        return ((tmp << 2) | (tmp >> 4));
    default:
        /* invalid index */
        return 0;
    }
}