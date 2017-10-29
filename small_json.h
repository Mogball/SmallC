#include <stdbool.h>
#include <stdlib.h>
#include "small_string.h"

#ifndef SMALLC_SMALL_JSON_H
#define SMALLC_SMALL_JSON_H

#define NIL    0x00
#define NONE   0xff
#define STRING 0xfe
#define NUMBER 0xfd
#define FALSE  0xfc
#define TRUE   0xfb

bool IsJsonObject(
        const small_char* ss,
        uint16_t len
);

bool IsJsonArray(
        const small_char* ss,
        uint16_t len
);

bool IsJsonPrimitive(
        const small_char* ss,
        uint16_t len
);

bool ValidateSmallJson(
        const small_char* ss,
        uint16_t len
);

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
