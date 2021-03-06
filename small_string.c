#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

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

bool IsNumeric(const small_char c) {
    return 0x20 <= c && c <= 0x29;
}

bool IsLetter(const small_char c) {
    return 0x01 <= c <= 0x1a;
}

void FromCString(
        small_char* const ss,
        const char* const s,
        const uint16_t len
) {
    for (uint16_t i = 0; i < len; i++) {
        uint8_t* bits = (uint8_t*) malloc(1);
        BitsForChar(s + i, bits);
        SmallStrSetChar(i, *bits, ss);
        free(bits);
    }
}

void ToCString(
        const small_char* const ss,
        char* const s,
        const uint16_t len
) {
    SegmentToCString(ss, 0, len, s);
}

void SegmentToCString(
        const small_char* const ss,
        const uint16_t start,
        const uint16_t end,
        char* const s
) {
    small_char bits;
    for (uint16_t i = start; i < end; i++) {
        bits = SmallStrCharAt(i, ss);
        CharForBits(s + i - start, &bits);
    }
}

void SmallStrCopy(
        small_char* const tgt,
        const small_char* const src,
        const uint16_t src_len
) {
    uint16_t small_len = (uint16_t) SmallStringSize(src_len);
    for (uint16_t i = 0; i < small_len; i++) {
        tgt[i] = (small_char) (tgt[i] & 0x00 | src[i]);
    }
}

void SegmentCopy(
        small_char* const tgt,
        const small_char* const src,
        const uint16_t tgt_start,
        const uint16_t src_start,
        const uint16_t len
) {
    for (uint16_t i = 0; i < len; i++) {
        SmallStrSetChar(tgt_start + i, SmallStrCharAt(src_start + i, src), tgt);
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
        SmallStrCopy(tgt, b, blen);
        return;
    }
    SmallStrCopy(tgt, a, alen);
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

bool SmallStrEquals(
        const small_char* const a,
        const small_char* const b,
        const uint16_t len) {
    uint16_t small_len = (uint16_t) SmallStringSize(len);
    uint8_t n_extra_bits = (uint8_t) (small_len * int_size - small_size * len);
    uint16_t i;
    for (i = 0; i < small_len - 1; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    uint8_t mask = (uint8_t) (0xff >> n_extra_bits);
    return (a[i] & mask) == (b[i] & mask);
}

bool SegmentsEqual(
        const small_char* const a,
        const small_char* const b,
        const uint16_t a_start,
        const uint16_t a_end,
        const uint16_t b_start,
        const uint16_t b_end
) {
    uint16_t len = a_end - a_start;
    if (b_end - b_start != len) {
        return false;
    }
    for (uint16_t i = 0; i < len; i++) {
        if (SmallStrCharAt(a_start + i, a) != SmallStrCharAt(b_start + i, b)) {
            return false;
        }
    }
    return true;
}

small_char SmallStrCharAt(
        const uint16_t i,
        const small_char* const ss
) {
    uint32_t char_start = (uint32_t) (i * small_size);
    uint16_t index = (uint16_t) (char_start / int_size);
    uint8_t mod = (uint8_t) (char_start % int_size);
    // TODO use a more general method to extract the bits
    switch(mod) {
        case 0:
            return (small_char) (ss[index] & 0x3f);
        case 2:
            return (small_char) (ss[index] >> 2);
        case 4:
            return (small_char) ((ss[index] >> 4) | (ss[index + 1] << 4) & 0x3f);
        case 6:
        default:
            return (small_char) ((ss[index] >> 6) | (ss[index + 1] << 2) & 0x3f);
    }
}

void SmallStrSetChar(const uint16_t i, const small_char bits, small_char* const ss) {
    uint8_t c = (uint8_t) (bits & 0x3f);
    uint32_t char_start = (uint32_t) (i * small_size);
    uint16_t index = (uint16_t) (char_start / int_size);
    uint8_t mod = (uint8_t) (char_start % int_size);
    // TODO use a more general method to set the bits
    switch(mod) {
        case 0:
            ss[index] = (small_char) (ss[index] & 0xc0 | c);
            break;
        case 2:
            ss[index] = (small_char) (ss[index] & 0x03 | (c << 2));
            break;
        case 4:
            ss[index] = (small_char) (ss[index] & 0x0f | (c << 4));
            ss[index + 1] = (small_char) (ss[index + 1] & 0xfc | (c >> 4));
            break;
        case 6:
        default:
            ss[index] = (small_char) (ss[index] & 0x3f | (c << 6));
            ss[index + 1] = (small_char) (ss[index + 1] & 0xf0 | (c >> 2));
            break;
    }
}

uint16_t SmallStrIndexOf(
        const small_char bits,
        const small_char* const ss,
        const uint16_t len) {
    return SegmentIndexOf(bits, ss, 0, len);
}

uint16_t SegmentIndexOf(
        const small_char bits,
        const small_char* const ss,
        const uint16_t start,
        const uint16_t end
) {
    for (uint16_t i = start; i < end; i++) {
        if (SmallStrCharAt(i, ss) == bits) {
            return i;
        }
    }
    return end;
}

void SmallSubStr(
        small_char* const tgt,
        const small_char* const ss,
        const uint16_t i,
        const uint16_t j
) {
    for (uint16_t k = i; k < j; k++) {
        SmallStrSetChar(k - i, SmallStrCharAt(k, ss), tgt);
    }
}

void _PopulateKmpTable(
        const small_char* const w,
        int16_t* const table,
        const uint16_t len
) {
    int16_t pos = 1;
    int16_t cnd = 0;
    table[0] = -1;
    while (pos < len) {
        if (SmallStrCharAt((const uint16_t) pos, w) == SmallStrCharAt((const uint16_t) cnd, w)) {
            table[pos] = table[cnd];
            pos++;
            cnd++;
        } else {
            table[pos] = cnd;
            cnd = table[cnd];
            while (cnd >= 0 && (SmallStrCharAt((const uint16_t) pos, w) != SmallStrCharAt((const uint16_t) cnd, w))) {
                cnd = table[cnd];
            }
            pos++;
            cnd++;
        }
    }
    table[pos]= cnd;
}

uint16_t _PerformKmpSearch(
        const small_char* const ss,
        const small_char* const w,
        const uint16_t ss_len,
        const uint16_t w_len,
        const int16_t* const table,
        uint16_t* const result,
        bool findFirst
) {
    int16_t m = 0;
    int16_t i = 0;
    int16_t nP = 0;
    while (m + i < ss_len) {
        if (SmallStrCharAt((const uint16_t) i, w) == SmallStrCharAt((const uint16_t) (m + i), ss)) {
            i++;
            if (i == w_len) {
                if (findFirst) {
                    return (uint16_t) m;
                }
                result[nP] = (uint16_t) m;
                nP++;
                m = m + i - table[i];
                i = table[i];
            }
        } else {
            if (table[i] > -1) {
                m = m + i - table[i];
                i = table[i];
            } else {
                m = (int16_t) (m + i + 1);
                i = 0;
            }
        }
    }
    if (findFirst) {
        return ss_len;
    } else {
        return (uint16_t) nP;
    }
}

uint16_t SmallStrIndexOfStr(
        const small_char* const ss,
        const small_char* const w,
        const uint16_t ss_len,
        const uint16_t w_len
) {
    if (w_len == 0) {
        return ss_len;
    }
    if (ss_len == 0) {
        return 0;
    }
    int16_t* table = (int16_t*) malloc(sizeof(int16_t) * w_len);
    _PopulateKmpTable(w, table, w_len);
    uint16_t index = _PerformKmpSearch(ss, w, ss_len, w_len, table, NULL, true);
    free(table);
    return index;
}

bool IsNumber(
        const small_char* const ss,
        const uint16_t len
) {
    if (ss == NULL || *ss == 0x00) {
        return false;
    }
    return IsSegmentFloat(ss, 0, len);
}

bool IsSegmentFloat(
        const small_char *const ss,
        const uint16_t start,
        const uint16_t end
) {
    if (end == start) {
        return false;
    }
    // TODO directly check the small string
    char* c_str = (char*) malloc((size_t) (end - start) + 1);
    c_str[end - start] = '\0';
    char* p;
    SegmentToCString(ss, start, end, c_str);
    strtof(c_str, &p);
    free(c_str);
    return *p == '\0';
}

bool IsSegmentInt(
        const small_char* const ss,
        const uint16_t start,
        const uint16_t end
) {
    if (end == start) {
        return false;
    }
    for (uint16_t i = start; i < end; i++) {
        small_char c = SmallStrCharAt(i, ss);
        if (c == MINUS) {
            if (i != start) {
                return false;
            }
        } else if (!IsNumeric(c)) {
            return false;
        }
    }
    return true;
}

float SegmentToFloat(
        const small_char *ss,
        uint16_t start,
        uint16_t end
) {
    if (end == start) {
        return 0;
    }
    char* c_str = (char*) malloc((size_t) (end - start) + 1);
    c_str[end - start] = '\0';
    char* p;
    SegmentToCString(ss, start, end, c_str);
    float result = strtof(c_str, &p);
    free(c_str);
    return result;
}

int16_t SegmentToInt(
        const small_char *ss,
        uint16_t start,
        uint16_t end
) {
    if (end == start) {
        return 0;
    }
    int n = 0;
    uint16_t i = start;
    bool neg = false;
    if (SmallStrCharAt(0, ss) == MINUS) {
        neg = true;
        i++;
    }
    for (; i < end; i++) {
        n = n * 10 + (SmallStrCharAt(i, ss) & 0x0f);
    }
    if (neg) {
        n = -n;
    }
    return (int16_t) n;
}

uint16_t IntCharLength(const int16_t n) {
    if (n == 0) {
        return 1;
    }
    int len = n < 0 ? 1 : 0;
    len += floor(log10(abs(n))) + 1;
    return (uint16_t) len;
}

void StringFromInt(
        int16_t n,
        small_char* const ss,
        const uint16_t len
) {
    uint16_t i = 0;
    uint8_t neg = 1;
    if (n < 0) {
        SmallStrSetChar(0, MINUS, ss);
        n = -n;
        i++;
        neg = 0;
    }
    for (; i < len; i++) {
        SmallStrSetChar((const uint16_t) (len - i - neg), (const small_char) ((n % 10) | 0x20), ss);
        n /= 10;
    }
}

uint16_t FloatCharLength(const float d) {
    return snprintf(NULL, 0, "%f", d);
}

void StringFromFloat(
        const float d,
        small_char* const ss,
        const uint16_t len
) {
    char* c_str = (char*) malloc(len + 1);
    snprintf(c_str, len + 1, "%f", d);
    FromCString(ss, c_str, len);
    free(c_str);
}

small_char* MakeSmallString(
        const char* const str,
        const uint16_t len
) {
    small_char* ss = (small_char*) malloc((size_t) SmallStringSize(len));
    FromCString(ss, str, len);
    return ss;
}

small_char* MakeString(uint16_t len) {
    return (small_char*) malloc((size_t) SmallStringSize((len)));
}

void DestroyString(small_char* ss) {
    free(ss);
}

bool IsBool(
        const small_char* const ss,
        const uint16_t len
) {
    if (len != 4 && len != 5) {
        return false;
    }
    if (len == 4) {
        return SmallStrEquals(ss, ssTRUE, 4);
    } else {
        return SmallStrEquals(ss, ssFALSE, 5);
    }
}

bool IsSegmentBool(
        const small_char* const ss,
        const uint16_t start,
        const uint16_t end
) {
    uint16_t len = end - start;
    if (len != 4 && len != 5) {
        return false;
    }
    if (len == 4) {
        return SegmentsEqual(ss, ssTRUE, start, end, 0, 4);
    } else {
        return SegmentsEqual(ss, ssFALSE, start, end, 0, 5);
    }
}

bool IsNull(
        const small_char* const ss,
        const uint16_t len
) {
    if (len != 4) {
        return false;
    }
    bool f = SmallStrEquals(ss, ssNULL, 4);
    return f;
}

bool IsSegmentNull(
        const small_char* const ss,
        const uint16_t start,
        const uint16_t end
) {
    if (end - start != 4) {
        return false;
    }
    return SegmentsEqual(ss, ssNULL, start, end, 0, 4);
}

void WriteAsBits(
        char* const out,
        const small_char* const ss,
        const uint16_t len
) {
    uint32_t ptr = 0;
    for (uint16_t i = 0; i < len; i++) {
        uint32_t start = (uint32_t) ((i + 1) * small_size - 1);
        uint32_t end = (uint32_t) (i * small_size);
        for (uint32_t j = start;; j--) {
            out[ptr++] = (char) (GetBit(ss, j) ? '1' : '0');
            if (j == end) {
                break;
            }
        }
    }
    out[ptr] = '\0';
}
