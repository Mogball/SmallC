#ifndef SMALLC_SMALL_STRING_H
#define SMALLC_SMALL_STRING_H

// http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html

#include <stdint.h>

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
 * Write the bits of the small string as a string representation.
 * The caller should allocate enough characters for each bit in
 * the small string plus one byte for the terminating null.
 * @param out the output string of bits
 * @param ss  the small string
 * @param len the small string length in characters
 */
void WriteAsBits(char* out, const small_char* ss, uint16_t len);

#endif //SMALLC_SMALL_STRING_H
