#include <stdlib.h>
#include "small_string.h"

void BitsForChar(
        const char* const c,
        uint8_t* const bits
) {
    if ('a' <= *c && *c <= 'z') {
        *bits = (uint8_t) (*c & 0x1f);
    } else if ('0' <= *c && *c <= '9') {
        *bits = (uint8_t) (*c & 0x0f | 0x20);
    } else if ('!' <= *c && *c <= '#') {
        *bits = (uint8_t) ((*c & 0x03 | 0x18) + 0x02);
    } else if ('%' <= *c && *c <= '&') {
        *bits = (uint8_t) ((*c & 0x07 | 0x18) + 0x01);
    } else if ('\'' <= *c && *c <= '/') {
        *bits = (uint8_t) (*c + 0x03);
    } else if (':' <= *c && *c <= '>') {
        *bits = (uint8_t) ((*c ^ 0x08) + 0x01);
    } else if ('[' == *c) {
        *bits = (uint8_t) 0x38;
    } else if (']' <= *c && *c <= '_') {
        *bits = (uint8_t) (*c ^ 0x64);
    } else if ('{' <= *c && *c <= '~') {
        *bits = (uint8_t) ((*c & 0x3f) + 0x01);
    } else {
        *bits = 0;
    }
}

void CharForBits(
        char* const c,
        const uint8_t* const bits
) {
    if (0x01 <= *bits && *bits <= 0x1a) {
        *c = (char) (*bits | 0x60);
    } else if (0x20 <= *bits && *bits <= 0x29) {
        *c = (char) (*bits | 0x10);
    } else if (0x1b <= *bits && *bits <= 0x1d) {
        *c = (char) ((*bits - 0x02) ^ 0x18 | 0x20);
    } else if (0x1e <= *bits && *bits <= 0x1f) {
        *c = (char) ((*bits - 0x01) ^ 0x18 | 0x20);
    } else if (0x2a <= *bits && *bits <= 0x32) {
        *c = (char) (*bits - 0x03);
    } else if (0x33 <= *bits && *bits <= 0x37) {
        *c = (char) ((*bits - 0x01) ^ 0x08);
    } else if (0x38 == *bits) {
        *c = '[';
    } else if (0x39 <= *bits && *bits <= 0x3b) {
        *c = (char) (*bits ^ 0x64);
    } else if (0x3c <= *bits && *bits <= 0x3f) {
        *c = (char) ((*bits - 0x01) | 0x40);
    } else {
        *c = '\0';
    }
}

void FromCString(
        small_char* const ss,
        const char* const s,
        const uint16_t len
) {
    uint16_t ptr = 0;
    for (uint16_t i = 0; i < len; i++) {
        uint8_t* bits = (uint8_t*) malloc(1);
        BitsForChar(s + i, bits);
        for (uint8_t bit_ptr = small_size - 1;; bit_ptr--) {
            uint8_t bit = (uint8_t) ((*bits >> bit_ptr) & 0x01);
            SetBit(ss, ptr, bit);
            ptr++;
            if (bit_ptr == 0) {
                break;
            }
        }
        free(bits);
    }
}

void ToCString(
        const uint8_t* const ss,
        char* const s,
        const uint16_t len
) {
    uint16_t ptr = 0;
    for (uint16_t i = 0; i < len; i++) {
        uint8_t* bits = (uint8_t*) malloc(1);
        *bits = 0;
        for (uint8_t bit_ptr = 0; bit_ptr < small_size; bit_ptr++) {
            uint8_t bit = (uint8_t) GetBit(ss, ptr);
            *bits = (uint8_t) (bit ? (*bits << 1) | 1 : *bits << 1);
            ptr++;
        }
        CharForBits(s + i, bits);
        free(bits);
    }
}

void SmallStrCpy(
        small_char* const tgt,
        const small_char* const src,
        const uint16_t src_len
) {
    uint16_t small_len = (uint16_t) SmallStringSize(src_len);
    for (uint16_t i = 0; i < small_len; i++) {
        tgt[i] = (small_char) (tgt[i] & 0x00 | src[i]);
    }
}

void SmallStrConcat(
        small_char* const tgt,
        const small_char* const a,
        const small_char* const b,
        const uint16_t alen,
        const uint16_t blen
) {
    if (alen == 0) {
        SmallStrCpy(tgt, b, blen);
        return;
    }
    SmallStrCpy(tgt, a, alen);
    if (blen == 0) {
        return;
    }
    uint8_t n_extra_bits = (uint8_t) (SmallStringSize(alen) * int_size - small_size * alen);
    uint16_t small_blen = (uint16_t) SmallStringSize(blen);
    uint16_t shift = (uint16_t) (int_size - n_extra_bits);
    uint16_t a_start = (uint16_t) (alen * small_size / int_size);
    uint16_t b_end = (uint16_t) (a_start + small_blen);
    tgt[a_start] &= (0xff >> n_extra_bits);
    for (uint16_t i = a_start; i < b_end; i++) {
        uint16_t spaced = b[i - a_start];
        spaced <<= shift;
        tgt[i] |= spaced;
        tgt[i + 1] = (small_char) (spaced >> 8);
    }
}

void WriteAsBits(
        char* const out,
        const small_char* const ss,
        const uint16_t len
) {
    uint32_t n_bits = (uint32_t) (small_size * len);
    uint32_t i;
    for (i = 0; i < n_bits; i++) {
        out[i] = (char) (GetBit(ss, i) ? '1' : '0');
    }
    out[i] = '\0';
}
