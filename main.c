#include <string.h>
#include <stdlib.h>
#include <printf.h>
#include "small_string.h"

int Test_FromCStringAndToCStringEmptyString() {
    char str[] = "";
    small_char* ss = (small_char*) malloc(small_size);
    char* nstr = (char*) malloc(small_size);
    FromCString(ss, str, 0);
    ToCString(ss, nstr, 0);
    uint8_t pass = (uint8_t) (strlen(nstr) == 0);
    free(nstr);
    free(ss);
    return pass;
}

int Test_FromCStringAndToCString() {
    char str[] = "abcdefghijklmnopqrstuvwxyz!\"#%&0123456789\'()*+,-./:;<=>[]^_{|}~";
    uint16_t len = (uint16_t) strlen(str);
    if (len != 63) {
        return 0;
    }
    uint16_t small_len = (uint16_t) SmallStringSize(len);
    if (small_len != 48) {
        return 0;
    }
    small_char* ss = (small_char*) malloc(small_len);
    char* nstr = (char*) malloc(len);
    FromCString(ss, str, len);
    ToCString(ss, nstr, len);
    uint8_t equal = 1;
    for (uint16_t i = 0; i < len; i++) {
        if (nstr[i] != str[i]) {
            equal = 0;
            break;
        }
    }
    free(ss);
    free(nstr);
    return equal & Test_FromCStringAndToCStringEmptyString();
}

int Test_SmallStrCpyEmptyString() {
    char str[] = "";
    small_char* ss = (small_char*) malloc(small_size);
    small_char* nss = (small_char*) malloc(small_size);
    FromCString(ss, str, 0);
    SmallStrCpy(nss, ss, 0);
    char* nstr = (char*) malloc(small_size);
    ToCString(nss, nstr, 0);
    uint8_t pass = (uint8_t) (strlen(nstr) == 0);
    free(ss);
    free(nss);
    free(nstr);
    return pass;
}

int Test_SmallStrCpy() {
    char str[] = "!\"#%&0123456789\'()*+,-./:;<=>[]^_{|}~";
    uint16_t len = (uint16_t) strlen(str);
    uint16_t small_len = (uint16_t) SmallStringSize(len);
    small_char* ss = (small_char*) malloc(small_len);
    FromCString(ss, str, len);
    small_char* tss = (small_char*) malloc(small_len);
    SmallStrCpy(tss, ss, len);
    char* nstr = (char*) malloc(len);
    ToCString(tss, nstr, len);
    uint8_t equal = 1;
    for (uint16_t i = 0; i < len; i++) {
        if (nstr[i] != str[i]) {
            equal = 0;
            break;
        }
    }
    free(ss);
    free(tss);
    free(nstr);
    return equal & Test_SmallStrCpyEmptyString();
}

int Test_SmallStrConcatCase(char* str1, char* str2, const char* expected) {
    uint16_t len1 = (uint16_t) strlen(str1);
    uint16_t len2 = (uint16_t) strlen(str2);
    small_char* ss1 = (small_char*) malloc((size_t) SmallStringSize(len1));
    small_char* ss2 = (small_char*) malloc((size_t) SmallStringSize(len2));
    FromCString(ss1, str1, len1);
    FromCString(ss2, str2, len2);
    small_char* target = (small_char*) malloc((size_t) SmallStringSize(len1 + len2));
    SmallStrConcat(target, ss1, ss2, len1, len2);
    char* result = (char*) malloc((size_t) len1 + len2);
    ToCString(target, result, len1 + len2);
    free(ss1);
    free(ss2);
    free(target);
    uint8_t equal = 1;
    for (uint16_t i = 0; i < len1 + len2; i++) {
        if (expected[i] != result[i]) {
            equal = 0;
            break;
        }
    }
    free(result);
    return equal;
}

int Test_SmallStringConcat() {
    int pass = 1;
    pass &= Test_SmallStrConcatCase("a", "b", "ab");
    pass &= Test_SmallStrConcatCase("a", "bc", "abc");
    pass &= Test_SmallStrConcatCase("ab", "c", "abc");
    pass &= Test_SmallStrConcatCase("abc", "d", "abcd");
    pass &= Test_SmallStrConcatCase("a", "bcd", "abcd");
    pass &= Test_SmallStrConcatCase("a", "bcde", "abcde");
    pass &= Test_SmallStrConcatCase("ab", "cdef", "abcdef");
    pass &= Test_SmallStrConcatCase("abc", "defgh", "abcdefgh");
    pass &= Test_SmallStrConcatCase("", "a", "a");
    pass &= Test_SmallStrConcatCase("", "abcde", "abcde");
    pass &= Test_SmallStrConcatCase("", "", "");
    pass &= Test_SmallStrConcatCase("a", "", "a");
    pass &= Test_SmallStrConcatCase("abcde", "", "abcde");
    return pass;
}

int Test_WriteAsBits() {
    char str[] = "^*34daf__";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = (small_char*) malloc((size_t) SmallStringSize(len));
    FromCString(ss, str, len);
    char expected[] = "111010101101100011100100000100000001000110111011111011";
    uint16_t expected_len = (uint16_t) strlen(expected);
    char* result = malloc((size_t) (2 * expected_len));
    WriteAsBits(result, ss, len);
    int equal = 1;
    for (uint16_t i = 0; i < expected_len; i++) {
        if (result[i] != expected[i]) {
            equal = 0;
            break;
        }
    }
    free(result);
    free(ss);
    return equal;
}

int Test_SmallStrEqualsCase(char* a, char* b, bool expected) {
    uint16_t len = (uint16_t) strlen(a);
    uint16_t small_len = (uint16_t) SmallStringSize(len);
    small_char* ss1 = (small_char*) malloc(small_len);
    small_char* ss2 = (small_char*) malloc(small_len);
    FromCString(ss1, a, len);
    FromCString(ss2, b, len);
    bool result = SmallStrEquals(ss1, ss2, len);
    free(ss1);
    free(ss2);
    return result == expected;
}

int Test_SmallStrEquals() {
    int pass = 1;
    pass &= Test_SmallStrEqualsCase("a", "a", true);
    pass &= Test_SmallStrEqualsCase("bb", "bb", true);
    pass &= Test_SmallStrEqualsCase("ccc", "ccc", true);
    pass &= Test_SmallStrEqualsCase("dddd", "dddd", true);
    pass &= Test_SmallStrEqualsCase("e", "a", false);
    pass &= Test_SmallStrEqualsCase("ea", "ae", false);
    pass &= Test_SmallStrEqualsCase("eee", "eef", false);
    pass &= Test_SmallStrEqualsCase("fdsfds", "sdfsdf", false);
    pass &= Test_SmallStrEqualsCase("eaas", "dfee", false);
    pass &= Test_SmallStrEqualsCase("12345*", "12345_", false);
    return pass;
}

int main() {
    printf("FromCStringAndToCString: %i\n", Test_FromCStringAndToCString());
    printf("SmallStrCpy: %i\n", Test_SmallStrCpy());
    printf("SmallStringConcat: %i\n", Test_SmallStringConcat());
    printf("WriteAsBits: %i\n", Test_WriteAsBits());
    printf("SmallStrEquals: %i\n", Test_SmallStrEquals());
}