#include <string.h>
#include <stdlib.h>
#include <printf.h>
#include "small_string.h"
#include "small_json.h"

int Do_TestEquals(int expected, int actual) {
    if (expected != actual) {
        printf("Expected %i but got %i\n", expected, actual);
        return 0;
    }
    return 1;
}

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

int Test_SmallStrIndexOf() {
    char str[] = "the_brown_fox_leaped_over_the_lazy_dog";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = (small_char*) malloc((size_t) SmallStringSize(len));
    char* c = (char*) malloc(1);
    small_char* sc = (small_char*) malloc(1);
    *c = 'l';
    BitsForChar(c, sc);
    FromCString(ss, str, len);
    bool pass = true;
    uint16_t i = SmallStrIndexOf(*sc, ss, len);
    if (i != 14) {
        pass = false;
    }
    *c = '^';
    BitsForChar(c, sc);
    i = SmallStrIndexOf(*sc, ss, len);
    if (i != len) {
        pass = false;
    }
    free(sc);
    free(c);
    free(ss);
    return pass;
}

int Test_SmallSubStr() {
    char str[] = "welcome_to_rapture";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = (small_char*) malloc((size_t) SmallStringSize(len));
    small_char* substr = (small_char*) malloc(SmallStringSize(4));
    FromCString(ss, str, len);
    SmallSubStr(substr, ss, 7, 11);
    char* result = (char*) malloc(len);
    char expected[] = "_to_";
    bool pass = true;
    ToCString(substr, result, 4);
    for (int i = 0; i < 4; i++) {
        if (result[i] != expected[i]) {
            pass = false;
        }
    }
    free(result);
    free(ss);
    return pass;
}

int Test_SmallStrIndexOfStr() {
    char str[] = "wel_to_rapcome_to_rapture";
    char substr[] = "_to_raptu";
    uint16_t len = (uint16_t) strlen(str);
    uint16_t sublen = (uint16_t) strlen(substr);
    small_char* ss = (small_char*) malloc((size_t) SmallStringSize(len));
    small_char* subss = (small_char*) malloc((size_t) SmallStringSize(sublen));
    bool pass = true;
    FromCString(ss, str, len);
    FromCString(subss, substr, sublen);
    uint16_t res = SmallStrIndexOfStr(ss, subss, len, sublen);
    if (res != 14) {
        pass = false;
    }
    free(ss);
    free(subss);
    return pass;
}

int Test_IsNumber() {
    char str1[] = "12341.514e10";
    char str2[] = "-35.033332345";
    char str3[] = "445hgfsl.";
    char str4[] = "5436625..e";
    uint16_t len = (uint16_t) strlen(str1);
    small_char* ss = MakeSmallString(str1, len);
    if (!IsNumber(ss, len)) {
        free(ss);
        return 0;
    }
    len = (uint16_t) strlen(str2);
    free(ss);
    ss = MakeSmallString(str2, len);
    if (!IsNumber(ss, len)) {
        free(ss);
        return 0;
    }
    len = (uint16_t) strlen(str3);
    free(ss);
    ss = MakeSmallString(str3, len);
    if (IsNumber(ss, len)) {
        free(ss);
        return 0;
    }
    len = (uint16_t) strlen(str4);
    free(ss);
    ss = MakeSmallString(str4, len);
    if (IsNumber(ss, len)) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_NextToken() {
    char str[] = "{\"hello\":\"my_name\",\"is\":[1,5,5.602,43.34e10],\"9\":true}";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    uint16_t index = len;
    uint16_t* pIndex = &index;
    if (!Do_TestEquals(NONE, _NextToken(ss, pIndex, len))) {
        free(ss);
        return 0;
    }
    index = 0;
    if (!Do_TestEquals(L_CRL, _NextToken(ss, pIndex, len)) || *pIndex != 1) {
        free(ss);
        return 0;
    }
    index = 8;
    if (!Do_TestEquals(COLON, _NextToken(ss, pIndex, len)) || *pIndex != 9) {
        free(ss);
        return 0;
    }
    index = 29;
    if (!Do_TestEquals(NUMBER, _NextToken(ss, pIndex, len)) || *pIndex != 30) {
        free(ss);
        return 0;
    }
    index = 49;
    if (!Do_TestEquals(TRUE, _NextToken(ss, pIndex, len)) || *pIndex != 53) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_GetLastIndexOfNumber() {
    char str[] = "___fjeajew-68385.3413fge";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    uint16_t index = 10;
    if (!Do_TestEquals(20, _GetLastIndexOfNumber(ss, index, len))) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_ValidateNumber() {
    char str[] = "__324..5424ee33..agsdf";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    uint16_t index = 2;
    if (!Do_TestEquals(false, _ValidateNumber(ss, &index, len)) || index != 17) {
        free(ss);
        return 0;
    }
    char str0[] = "{{[\"\":-3453.3434gaf]gr}}";
    len = (uint16_t) strlen(str0);
    free(ss);
    ss = MakeSmallString(str0, len);
    index = 0;
    uint16_t token = NONE;
    while (token != NUMBER && index < len){
        token = _NextToken(ss, &index, len);
    }
    index--;
    if (!Do_TestEquals(true, _ValidateValue(ss, &index, len)) || index != 16) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_ValidateString() {
    char str[] = "{{\"hello\":\"my_name_is\"}}";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    uint16_t index = 0;
    while(_NextToken(ss, &index, len) != STRING && index < len);
    if (!Do_TestEquals(true, _ValidateString(ss, &index, len)) || index != 9) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_ValidateArray() {
    char str[] = "{\"array\":[\"hello\",\"my_name\",\"is\",\"jeff\"],\"value\":34.63}";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    uint16_t index = 9;
    if (!Do_TestEquals(true, _ValidateArray(ss, &index, len)) || index != 40) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_ValidateObject() {
    char str[] = "{\"array\":[\"hello\",\"my_name\",\"is\",\"jeff\"],\"value\":34.63}";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    uint16_t index = 0;
    if (!Do_TestEquals(true, _ValidateObject(ss, &index, len)) || index != len) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_ValidateJson() {
    char str[] = "{\"items\":{\"item\":[{\"id\":\"0001\",\"type\":\"donut\",\"name\":null,\"ppu\":0.55,"
            "\"batters\":{\"batter\":[{\"id\":\"1001\",\"type\":\"regular\"},{\"id\":\"1002\",\"type\":\"ch"
            "ocolate\"},{\"id\":\"1003\",\"type\":\"blueberry\"},{\"id\":\"1004\",\"type\":\"devils_food\"}"
            "]},\"topping\":[{\"id\":\"5001\",\"type\":\"none\"},{\"id\":\"5002\",\"type\":\"glazed\"},{\"i"
            "d\":\"5005\",\"type\":\"sugar\"},{\"id\":true,\"type\":\"powdered_sugar\"},{\"id\":\"5006"
            "\",\"type\":\"chocolate_with_sprinkles\"},{\"id\":\"5003\",\"type\":\"chocolate\"},{\"id\":\""
            "5004\",\"type\":\"maple\"}]}]}}";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    if (!Do_TestEquals(true, ValidateSmallJson(ss, len))) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_ValidateJsonInvalid() {
    char str[] = "{\"items\":{\"item\":[{\"id\":\"0001\",\"type\":\"donut\",\"name\":\"cake\",\"ppu\":0.55,"
            "\"batters\":{\"batter\":[{\"id\":\"1001\",\"type\":\"regular\"},{\"id\":\"1002\",\"type\":\"ch"
            "ocolate\"},{\"id\":\"1003\",\"type\":\"blueberry\"},{\"id\":\"1004\",\"type\":\"devils_food\"}"
            "]},\"topping\":[{\"id\":\"5001\",\"type\":\"none\"}},{\"id\":\"5002\",\"type\":\"glazed\"},{\"i"
            "d\":\"5005\",\"type\":\"sugar\"},{\"id\":\"5007\",\"type\":\"powdered_sugar\"},{\"id\":\"5006"
            "\",\"type\":\"chocolate_with_sprinkles\"},{\"id\":\"5003\",\"type\":\"chocolate\"},{\"id\":\""
            "5004\",\"type\":\"maple\"}]}]}}";
    uint16_t len = (uint16_t) strlen(str);
    small_char* ss = MakeSmallString(str, len);
    if (!Do_TestEquals(false, ValidateSmallJson(ss, len))) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int Test_IsBool() {
    small_char* ss = MakeSmallString("true", 4);
    if (!IsBool(ss, 4)) {
        free(ss);
        return 0;
    }
    free(ss);
    ss = MakeSmallString("make", 4);
    if (IsBool(ss, 4)) {
        free(ss);
        return 0;
    }
    if (IsNull(ss, 4)) {
        free(ss);
        return 0;
    }
    free(ss);
    ss = MakeSmallString("false", 5);
    if (!IsBool(ss, 5)) {
        free(ss);
        return 0;
    }
    free(ss);
    ss = MakeSmallString("gucci", 5);
    if (IsBool(ss, 5)) {
        free(ss);
        return 0;
    }
    free(ss);
    ss = MakeSmallString("null", 4);
    if (!IsNull(ss, 4)) {
        free(ss);
        return 0;
    }
    free(ss);
    return 1;
}

int main() {
    printf("FromCStringAndToCString: %i\n", Test_FromCStringAndToCString());
    printf("SmallStrCpy: %i\n", Test_SmallStrCpy());
    printf("SmallStringConcat: %i\n", Test_SmallStringConcat());
    printf("WriteAsBits: %i\n", Test_WriteAsBits());
    printf("SmallStrEquals: %i\n", Test_SmallStrEquals());
    printf("SmallStrIndexOf: %i\n", Test_SmallStrIndexOf());
    printf("SmallSubStr: %i\n", Test_SmallSubStr());
    printf("SmallStrIndexOfStr: %i\n", Test_SmallStrIndexOfStr());
    printf("IsNumber: %i\n", Test_IsNumber());
    printf("_NextToken: %i\n", Test_NextToken());
    printf("_GetLastIndexOfNumber: %i\n", Test_GetLastIndexOfNumber());
    printf("_ValidateNumber: %i\n", Test_ValidateNumber());
    printf("_ValidateString: %i\n", Test_ValidateString());
    printf("_ValidateArray: %i\n", Test_ValidateArray());
    printf("_ValidateObject: %i\n", Test_ValidateObject());
    printf("ValidateSmallJson: %i\n", Test_ValidateJson());
    printf("ValidateSmallJsonInvalid: %i\n", Test_ValidateJsonInvalid());
    printf("IsBool: %i\n", Test_IsBool());
}