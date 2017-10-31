#include <string.h>
#include <stdlib.h>
#include <printf.h>
#include <math.h>
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
    SmallStrCopy(nss, ss, 0);
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
    SmallStrCopy(tss, ss, len);
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

int Test_HashTable() {
    HashTable* table = MakeTable(5, 0.75f);
    uint16_t vals[12][2] = {
            {45, 5321},
            {21, 6883},
            {67, 4512},
            {10, 3433},
            {11, 1111},
            {90, 5023},
            {78, 5566},
            {4332, 32451},
            {6653, 12354},
            {9583, 59304},
            {6737, 63752},
            {4573, 33322},
    };
    for (uint16_t i = 0; i < 12; i++) {
        TablePut(table, vals[i][0], vals[i][1]);
    }
    for (uint16_t i = 0; i < 12; i++) {
        uint16_t result = TableGet(table, vals[i][0]);
        if (result != vals[i][1]) {
            printf("%i: Expected %i but got %i with key %i", i, vals[i][1], result, vals[i][0]);
            DestroyTable(table);
            return 0;
        }
    }
    DestroyTable(table);
    return 1;
}

int Test_JsonObjectGetValue() {
    char json[] = "{\"items\":[1,2,3,4,5],\"key\":\"value\",\"gucci\":{\"a\":\"letter_a\",\"b\":\"letter_b\""
            ",\"something\":{\"16\":-1434.23,\"always\":true}}}";
    uint16_t len = (uint16_t) strlen(json);
    small_char* ss = MakeSmallString(json, len);
    HashTable* index = IndexJson(ss, len);
    if (!ValidateSmallJson(ss, len)) {
        free(ss);
        DestroyTable(index);
        return 0;
    }
    JsonElement element = AsJsonElement(ss, len);
    if (!IsJsonObject(&element)) {
        free(ss);
        DestroyTable(index);
        return 0;
    }
    small_char* key = MakeSmallString("items", 5);
    JsonElement array = JsonObjectGetValue(&element, index, key, 5);
    if (array.end != 20 || array.start != 9 || !IsJsonArray(&array) || GetElementType(&array) != ARRAY) {
        free(ss);
        free(key);
        DestroyTable(index);
        return 0;
    }
    free(key);
    key = MakeSmallString("key", 3);
    JsonElement string = JsonObjectGetValue(&element, index, key, 3);
    if (string.start != 27 || string.end != 34 || !IsJsonString(&string) || GetElementType(&string) != STRING) {
        free(ss);
        free(key);
        DestroyTable(index);
        return 0;
    }
    uint16_t str_len = JsonStringLength(&string);
    small_char* str_val = (small_char*) malloc((size_t) SmallStringSize(str_len));
    JsonStringGet(&string, str_val);
    char* c_str_val = (char*) malloc(str_len + 1);
    ToCString(str_val, c_str_val, str_len);
    c_str_val[str_len] = '\0';
    char expected_str[] = "value";
    free(str_val);
    free(key);
    for (uint16_t i = 0; i < str_len; i++) {
        if (expected_str[i] != c_str_val[i]) {
            free(c_str_val);
            free(ss);
            DestroyTable(index);
            return 0;
        }
    }
    free(c_str_val);
    key = MakeSmallString("gucci", 5);
    JsonElement object = JsonObjectGetValue(&element, index, key, 5);
    if (!IsJsonObject(&object) || object.start != 43 || object.end != 116 || GetElementType(&object) != OBJECT) {
        free(ss);
        free(key);
        DestroyTable(index);
        return 0;
    }
    free(key);
    key = MakeSmallString("something", 9);
    JsonElement subobj = JsonObjectGetValue(&object, index, key, 9);
    if (
            !JsonObjectContainsKey(&object, index, key, 9) ||
            !IsJsonObject(&subobj) ||
            GetElementType(&subobj) != OBJECT ||
            subobj.start != 86 ||
            subobj.end != 115
    ) {
        DestroyTable(index);
        free(ss);
        free(key);
        return 0;
    }
    free(key);
    key = MakeSmallString("16", 2);
    JsonElement number = JsonObjectGetValue(&subobj, index, key, 2);
    if (GetElementType(&number) != NUMBER || number.start != 92 || number.end != 100) {
        DestroyTable(index);
        free(ss);
        free(key);
        return 0;
    }
    DestroyTable(index);
    free(ss);
    free(key);
    return 1;
}

int Test_JsonArrayElementAt() {
    char str[] = "{\"array\":[-123.34,\"hello\",{\"a\":\"letter_a\",\"b\":\"letter_b\"},false]}";
    uint16_t len = (uint16_t) strlen(str);
    small_char* json = MakeSmallString(str, len);
    bool pass = true;
    if (!ValidateSmallJson(json, len)) {
        free(json);
        return 0;
    }
    small_char* key = MakeSmallString("array", 5);
    JsonElement element = AsJsonElement(json, len);
    HashTable* index = IndexJson(json, len);
    JsonElement array = JsonObjectGetValue(&element, index, key, 5);
    free(key);
    if (!IsJsonArray(&array)) {
        free(json);
        DestroyTable(index);
        return 0;
    }
    JsonElement number = JsonArrayElementAt(&array, index, 0);
    uint8_t type = GetPrimitiveType(&number);
    float val = JsonFloatGet(&number);
    if (type != NUMBER || val != -123.34f) {
        free(json);
        DestroyTable(index);
        return 0;
    }
    JsonElement string = JsonArrayElementAt(&array, index, 1);
    type = GetPrimitiveType(&string);
    if (type != STRING) {
        free(json);
        DestroyTable(index);
        return 0;
    }
    uint16_t str_len = JsonStringLength(&string);
    small_char* str_val = (small_char*) malloc((size_t) SmallStringSize(str_len));
    JsonStringGet(&string, str_val);
    char* c_str_val = (char*) malloc(str_len + 1);
    ToCString(str_val, c_str_val, str_len);
    free(str_val);
    c_str_val[str_len] = '\0';
    char expected_str[] = "hello";
    for (uint16_t i = 0; i < str_len; i++) {
        if (expected_str[i] != c_str_val[i]) {
            free(c_str_val);
            free(json);
            DestroyTable(index);
            return 0;
        }
    }
    free(c_str_val);
    JsonElement boolean = JsonArrayElementAt(&array, index, 3);
    if (GetPrimitiveType(&boolean) != BOOL || JsonBoolGet(&boolean) != false) {
        free(json);
        DestroyTable(index);
        return 0;
    }
    JsonElement subobj = JsonArrayElementAt(&array, index, 2);
    if (
            !IsJsonObject(&subobj) ||
            JsonObjectCountKeys(&subobj, index) != 2 ||
            JsonObjectKeyAt(&subobj, index, 0).start != 27
    ) {
        free(json);
        DestroyTable(index);
        return 0;
    }
    free(json);
    DestroyTable(index);
    return 1;
}

int Test_JsonArrayCompile() {
    int16_t e1 = -19673;
    float e2 = -423.4f;
    small_char* e3 = MakeSmallString("someval", 7);
    uint16_t l1 = IntCharLength(e1);
    uint16_t l2 = FloatCharLength(e2);
    uint16_t l3 = 7;
    small_char* str1 = MakeString(l1);
    small_char* str2 = MakeString(l2);
    small_char* str3 = MakeString(l3);
    JsonElement je1 = IntAsJsonElement(e1, str1, l1);
    JsonElement je2 = FloatAsJsonElement(e2, str2, l2);
    JsonElement je3 = StringAsJsonElement(e3, str3, l3);
    ArrayList* list = MakeList(3);
    ListAppend(list, je1);
    ListAppend(list, je2);
    ListAppend(list, je3);
    uint16_t len = JsonArrayCompileLength(list);
    small_char* ss = MakeString(len);
    JsonElement arr = JsonArrayCompile(list, ss, len);
    char* cstr = (char*) malloc(len + 1);
    ToCString(arr.json, cstr, len);
    cstr[len] = '\0';
    char expected[] = "[-19673,-423.399994,\"someval\"]";
    bool pass = true;
    for (uint16_t i = 0; i < len; i++) {
        if (expected[i] != cstr[i]) {
            pass = false;
        }
    }
    if (!ValidateSmallJson(arr.json, arr.end - arr.start)) {
        pass = false;
    }
    free(cstr);
    free(ss);
    free(e3);
    free(str1);
    free(str2);
    free(str3);
    DestroyList(list);
    return pass;
}

int Test_JsonObjectCompile() {
    small_char* ss1 = MakeSmallString("\"key1\"", 6);
    small_char* ss2 = MakeSmallString("\"key2\"", 6);
    small_char* ss3 = MakeSmallString("\"key3\"", 6);
    small_char* sval1 = MakeSmallString("\"value1\"", 8);
    small_char* sval2 = MakeSmallString("\"value2\"", 8);
    small_char* sval3 = MakeSmallString("\"value3\"", 8);
    JsonElement key1 = AsJsonElement(ss1, 6);
    JsonElement key2 = AsJsonElement(ss2, 6);
    JsonElement key3 = AsJsonElement(ss3, 6);
    JsonElement value1 = AsJsonElement(sval1, 8);
    JsonElement value2 = AsJsonElement(sval2, 8);
    JsonElement value3 = AsJsonElement(sval3, 8);
    HashMap* map = MakeMap(5, 0.75f);
    MapPut(map, key1, value1);
    MapPut(map, key2, value2);
    MapPut(map, key3, value3);
    bool pass = true;
    if (MapGet(map, key1).json != value1.json) {
        pass = false;
    }
    if (MapGet(map, key2).json != value2.json) {
        pass = false;
    }
    if (MapGet(map, key3).json != value3.json) {
        pass = false;
    }
    uint16_t object_len = JsonObjectCompileLength(map);
    small_char* ss = MakeString(object_len);
    JsonElement object = JsonObjectCompile(map, ss, object_len);
    char* cstr = (char*) malloc(object_len);
    ToCString(object.json, cstr, object_len);
    char expected[] = "{\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}";
    for (uint16_t i = 0; i < object_len; i++) {
        if (expected[i] != cstr[i]) {
            pass = false;
            break;
        }
    }
    if (!ValidateSmallJson(object.json, object.end - object.start));
    free(cstr);
    DestroyMap(map);
    free(ss);
    free(sval1);
    free(sval2);
    free(sval3);
    free(ss1);
    free(ss2);
    free(ss3);
    return pass;
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
    printf("IsBool: %i\n", Test_IsBool());
    printf("HashTable: %i\n", Test_HashTable());
    printf("_NextToken: %i\n", Test_NextToken());
    printf("_GetLastIndexOfNumber: %i\n", Test_GetLastIndexOfNumber());
    printf("_ValidateNumber: %i\n", Test_ValidateNumber());
    printf("_ValidateString: %i\n", Test_ValidateString());
    printf("_ValidateArray: %i\n", Test_ValidateArray());
    printf("_ValidateObject: %i\n", Test_ValidateObject());
    printf("ValidateSmallJson: %i\n", Test_ValidateJson());
    printf("ValidateSmallJsonInvalid: %i\n", Test_ValidateJsonInvalid());
    printf("JsonObject: %i\n", Test_JsonObjectGetValue());
    printf("JsonArray: %i\n", Test_JsonArrayElementAt());
    printf("JsonArrayCompile: %i\n", Test_JsonArrayCompile());
    printf("JsonObjectCompile: %i\n", Test_JsonObjectCompile());
}