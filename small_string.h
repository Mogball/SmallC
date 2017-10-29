#ifndef SMALLC_SMALL_STRING_H
#define SMALLC_SMALL_STRING_H

// http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html

#include <stdint.h>
#include <stdbool.h>

#define small_size 6
#define int_size 8

#define AssignBit(A, k) ((A)[(k)/int_size] |=  (1 << ((k)%int_size)))
#define ClearBit(A, k)  ((A)[(k)/int_size] &= ~(1 << ((k)%int_size)))
#define GetBit(A, k)    ((A)[(k)/int_size] &   (1 << ((k)%int_size)))
#define SetBit(A, k, b) b ? AssignBit(A, k) : ClearBit(A, k)

/**
 * Obtain the size in bytes of a small string given the
 * number of 8-bit characters it would represent. The
 * value is rounded to the nearest size of the small
 * string element defined below.
 */
#define SmallStringSize(n) (((n) * small_size + int_size - 1) / int_size)

/**
 * Gets the 6-bit representation of a character.
 * If the passed character is not in the defined
 * lexicon of 64 characters, behaviour is undefined.
 * Caller should allocate space for the resultant bits.
 * @param c    a valid 8-bit character
 * @param bits the 6-bit representation
 */
void BitsForChar(const char* c, uint8_t* bits);
/**
 * Gets the corresponding 8-bit representation
 * of a character in its 6-bit representation.
 * Caller should allocate space for the resultant character.
 * @param c    the 8-bit representation
 * @param bits a 6-bit character between 0x00 and 0x3f
 */
void CharForBits(char* c, const uint8_t* bits);

/**
 * The element of a small string. Small string
 * 6-bit characters are inserted into an array
 * of these elements.
 */
typedef uint8_t small_char;

#define L_CRL 0x3c
#define R_CRL 0x3e
#define L_SQR 0x38
#define R_SQR 0x39
#define COLON 0x33
#define COMMA 0x2f
#define QUOTE 0x1c
#define MINUS 0x30
#define PLUS  0x2e
#define DOT   0x31

#define A 0x01
#define E 0x05
#define F 0x06
#define L 0x0c
#define N 0x0e
#define R 0x12
#define S 0x13
#define T 0x14
#define U 0x15

#define ssTRUE  ((const small_char *const) "\x94\x54\x15")
#define ssFALSE ((const small_char *const) "\x46\xc0\x4c\x05")
#define ssNULL  ((const small_char *const) "\x4e\xc5\x30")

/**
 * Convert a basic C string into a small string. The caller
 * should allocate the space for the small string, given
 * in bytes by the macro {@code SmallStringSize(len)}.
 * @param ss  a small string reprensetation of the C string
 * @param s   the C string to convert
 * @param len the length in characters of the C string
 */
void FromCString(small_char* ss, const char* s, uint16_t len);
/**
 * Convert a small string into a basic C string. The caller
 * should allocate the space for the C string, as the number
 * of bytes equal to the number of characters.
 * @param ss  the small string to convert
 * @param s   a C string representation of the small string
 * @param len the number of characters in the small string
 */
void ToCString(const small_char* ss, char* s, uint16_t len);

/**
 * Copy a small string into space allocated for another
 * small string. The caller should pass the length
 * in characters of the small string. The target string
 * memory should be allocated as
 * {@code SmallStringSize(src_len)}.
 * @param tgt     the target memory to copy
 * @param src     the source small string
 * @param src_len the number of characters in the source string
 */
void SmallStrCpy(small_char* tgt, const small_char* src, uint16_t src_len);

/**
 * Concatenate two small strings into a target small string.
 * Caller must allocate sufficient space in the target
 * small string, where
 * {@code SmallStringSize(tgt) = SmallStringSize(a + b).
 * The result is equivalent to {@code a + b}.
 * @param tgt target string holding the concatenated string
 * @param a   first string
 * @param b   second string
 */
void SmallStrConcat(small_char* tgt, const small_char* a, const small_char* b, uint16_t alen, uint16_t blen);

/**
 * This method compares two small strings
 * and returns whether they are equal. The caller
 * is expected to have checked the lengths.
 * @param a   the first string
 * @param b   the second string
 * @param len the length of each string
 * @return a byte 0 if unequal and 1 if equal
 */
bool SmallStrEquals(const small_char* a, const small_char* b, uint16_t len);

/**
 * Returns the small character bits guaranteed
 * to be in the form {@code 00xxxxxx} at a specified
 * character index in the short string. The provided
 * index is expected to be less than the character
 * length of the small string.
 * @param i  the character index to grab the bits
 * @param ss the small string
 * @return   the 6 bits of the character
 */
small_char SmallStrCharAt(uint16_t i, const small_char* ss);

/**
 * Set the character at the given character index
 * to the 6 bits specified. The caller is expected
 * to choose a valid character index, less than the
 * character length of the string.
 * @param i    the character index to set the bits
 * @param bits the bits representing the 6-bit character
 * @param ss   the small string to modify
 */
void SmallStrSetChar(uint16_t i, small_char bits, small_char* ss);

/**
 * Finds the first character index of a 6-bit
 * character of the proper form in a small string.
 * If the character is not found, returns the length
 * @param c   the 6 bits of the character to search
 * @param ss  the small string to search
 * @param len the length of the small string
 * @return the character index of the first occurrence or the length
 */
uint16_t SmallStrIndexOf(small_char bits, const small_char* ss, uint16_t len);

uint16_t SegmentIndexOf(
        small_char bits,
        const small_char* ss,
        uint16_t start,
        uint16_t end
);

/**
 * Find and store a substring of a small string from
 * a start character index and end index in a
 * target small string. The range includes the start index
 * and excludes the end index. The caller is expected to have
 * allocated the appropriate memory of the target string.
 * @param tgt the target string to write the substring
 * @param ss  the source string to take the substring
 * @param i   the start index, inclusive
 * @param j   the end index, exclusive
 */
void SmallSubStr(small_char* tgt, const small_char* ss, uint16_t i, uint16_t j);

void _PopulateKmpTable(
        const small_char* w,
        int16_t* table,
        uint16_t len
);

uint16_t _PerformKmpSearch(
        const small_char* ss,
        const small_char* w,
        uint16_t ss_len,
        uint16_t w_len,
        const int16_t* table,
        uint16_t* result,
        bool findFirst
);

uint16_t SmallStrIndexOfStr(
        const small_char* ss,
        const small_char* w,
        uint16_t ss_len,
        uint16_t w_len
);

bool IsNumeric(small_char c);
bool IsLetter(small_char c);
bool IsNumber(const small_char* ss, uint16_t len);
bool IsBool(const small_char* ss, uint16_t len);
bool IsNull(const small_char* ss, uint16_t len);
small_char* MakeSmallString(const char* str, uint16_t len);

bool AreSegmentsEqual(
        const small_char* a,
        const small_char* b,
        uint16_t a_start, // inclusive
        uint16_t a_end,   // exclusive
        uint16_t b_start, // inclusive
        uint16_t b_end    // exclusive
);

bool IsSegmentNumber(
        const small_char* ss,
        uint16_t start,
        uint16_t end
);

bool IsSegmentBool(
        const small_char* ss,
        uint16_t start,
        uint16_t end
);

bool IsSegmentNull(
        const small_char* ss,
        uint16_t start,
        uint16_t end
);

void SegmentToCString(
        const small_char* ss,
        uint16_t start,
        uint16_t end,
        char* s
);

/**
 * Write the bits of the small string as a string representation.
 * The caller should allocate enough characters for each bit in
 * the small string plus one byte for the terminating null.
 * @param out the output string of bits
 * @param ss  the small string
 * @param len the small string length in characters
 */
void WriteAsBits(char* out, const small_char* ss, uint16_t len);

#endif //SMALLC_SMALL_STRING_H
