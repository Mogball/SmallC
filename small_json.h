#include <stdbool.h>
#include <stdlib.h>
#include "small_string.h"
#include "hashtable.h"

#ifndef SMALLC_SMALL_JSON_H
#define SMALLC_SMALL_JSON_H

#define NIL    0xf9
#define NONE   0xff
#define STRING 0xfe
#define NUMBER 0xfd
#define FALSE  0xfc
#define TRUE   0xfb
#define BOOL   0xfa
#define OBJECT 0xf8
#define ARRAY  0xf7

/**
 * Users are encouraged to call this function on a
 * string before any other functions. Most following functions
 * will assume that this function has been called on the
 * backing small string of a JsonElement. This function
 * checks if the specified small string is a valid JSON string.
 * @param ss  the small string the validate
 * @param len the length of the element
 * @return true if the string is a valid JSON string
 */
bool ValidateSmallJson(
        const small_char* ss,
        uint16_t len
);

typedef struct JsonElement {
    const small_char* json; // backing small string
    uint16_t start;         // element start index, inclusive
    uint16_t end;           // element end index, exclusive
} JsonElement;

/**
 * Build an index consisting of element start positions
 * mapped to element end positions. Constructing the index
 * is always O(n) and dramatically speeds ups further
 * operations on the base string. This function assumes a
 * valid JSON format. The caller must destroy the table
 * when finished with it.
 * @param json the backing JSON string
 * @param len  the length of the string
 * @return a map of element start indices inclusive to end indices exclusive
 */
HashTable* IndexJson(const small_char* json, uint16_t len);

/**
 * Create a JSON element using the backing string. The string
 * is used for all further manipulations are should not be
 * modified.
 * @param json the backing JSON string
 * @param len  the length of the string
 * @return a new JSON element view of the string
 */
JsonElement AsJsonElement(const small_char* json, uint16_t len);

bool IsJsonObject(const JsonElement* element);

bool IsJsonArray(const JsonElement* element);

bool IsJsonString(const JsonElement* element);

bool IsJsonPrimitive(const JsonElement* element);

uint8_t GetPrimitiveType(const JsonElement* element);

uint8_t GetElementType(const JsonElement* element);

bool JsonObjectContainsKey(
        const JsonElement* object,
        const HashTable* table,
        const small_char* key,
        uint16_t key_len
);

JsonElement JsonObjectGetValue(
        const JsonElement* object,
        const HashTable* table,
        const small_char* key,
        uint16_t key_len
);

uint16_t JsonObjectCountKeys(
        const JsonElement* object,
        const HashTable* table
);

JsonElement JsonObjectKeyAt(
        const JsonElement* object,
        const HashTable* table,
        uint16_t pos
);

uint16_t JsonArraySize(
        const JsonElement* array,
        const HashTable* table
);

JsonElement JsonArrayElementAt(
        const JsonElement* array,
        const HashTable* table,
        uint16_t i
);

uint16_t JsonStringLength(const JsonElement* string);

void JsonStringGet(const JsonElement* string, small_char* ss);

float JsonNumberGet(const JsonElement* number);

bool JsonBoolGet(const JsonElement* boolean);

bool JsonIsNull(const JsonElement* null);

bool _ValidateObject(
        const small_char* json,
        uint16_t* index,
        uint16_t len
);

bool _ValidateArray(
        const small_char* json,
        uint16_t* index,
        uint16_t len
);

bool _ValidateValue(
        const small_char* json,
        uint16_t* index,
        uint16_t len
);

bool _ValidateString(
        const small_char* json,
        uint16_t* index,
        uint16_t len
);

bool _ValidateNumber(
        const small_char* json,
        uint16_t* index,
        uint16_t len
);

uint16_t _GetLastIndexOfNumber(
        const small_char* json,
        uint16_t index,
        uint16_t len
);

small_char _LookAhead(
        const small_char* json,
        uint16_t index,
        uint16_t len
);

small_char _NextToken(
        const small_char* json,
        uint16_t* index,
        uint16_t len
);

#endif //SMALLC_SMALL_JSON_H
