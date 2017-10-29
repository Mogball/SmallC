#include <stdio.h>
#include "small_json.h"
#include "stack.h"

HashTable* IndexJson(
        const small_char* const json,
        const uint16_t len
) {
    Stack* stack = MakeStack(len);
    uint16_t elements = 0;
    for (uint16_t i = 0; i < len; i++) {
        small_char c = SmallStrCharAt(i, json);
        if (c == L_CRL || c == L_SQR) {
            Push(stack, c);
        } else if (c == QUOTE) {
            if (Peek(stack) != QUOTE) {
                Push(stack, c);
            } else {
                Pop(stack);
                elements++;
            }
        } else if (c == R_CRL || c == R_SQR) {
            Pop(stack);
            elements++;
        } else if (IsNumeric(c) && Peek(stack) != QUOTE) {
            i = _GetLastIndexOfNumber(json, i, len);
            elements++;
        } else if ((c == N || c == T || c == F) && Peek(stack) != QUOTE) {
            i += c == F ? 4 : 3;
            elements++;
        }
    }
    HashTable* table = MakeTable((uint16_t) (elements * 1.618f));
    DestroyStack(stack);
    Stack* startPos = MakeStack((uint16_t) (elements * 2));
    stack = MakeStack((uint16_t) (elements * 2));
    for (uint16_t i = 0; i < len; i++) {
        small_char c = SmallStrCharAt(i, json);
        if (c == L_CRL || c == L_SQR) {
            Push(stack, c);
            Push(startPos, i);
        } else if (c == QUOTE) {
            if (Peek(stack) != QUOTE) {
                Push(stack, c);
                Push(startPos, i);
            } else {
                Pop(stack);
                TablePut(table, Pop(startPos), i);
            }
        } else if (c == R_CRL || c == R_SQR) {
            Pop(stack);
            TablePut(table, Pop(startPos), i);
        } else if (IsNumeric(c) && Peek(stack) != QUOTE) {
            uint16_t end = _GetLastIndexOfNumber(json, i, len);
            TablePut(table, i, end);
            i = end;
        } else if ((c == N || c == T || c == F) && Peek(stack) != QUOTE) {
            uint16_t end = (uint16_t) (i + (c == F ? 4 : 3));
            TablePut(table, i, end);
            i = end;
        }
    }
    DestroyStack(startPos);
    DestroyStack(stack);
    return table;
}

bool JsonObjectContainsKey(
        const JsonElement* const object,
        const HashTable* const table,
        const small_char* const key
) {
    uint16_t i = (uint16_t) (object->start + 1);
    while (i < object->end) {
        uint16_t end = TableGet(table, i);

    }
}

JsonElement AsJsonElement(
        const small_char* const json,
        const uint16_t len
) {
    JsonElement element;
    element.json = json;
    element.start = 0;
    element.end = len;
    return element;
}

bool IsJsonObject(const JsonElement* const element) {
    if (SmallStrCharAt(element->start, element->json) != L_CRL) {
        return false;
    }
    if (SmallStrCharAt(element->end, element->json) != R_CRL) {
        return false;
    }
    return true;
}

bool IsJsonArray(const JsonElement* const element) {
    if (SmallStrCharAt(element->start, element->json) != L_SQR) {
        return false;
    }
    if (SmallStrCharAt(element->end, element->json) != R_SQR) {
        return false;
    }
    return true;
}

bool IsJsonString(const JsonElement* const element) {
    if (element->end - element->start < 2) {
        return false;
    }
    if (SmallStrCharAt(element->start, element->json) != QUOTE) {
        return false;
    }
    if (SmallStrCharAt(element->end, element->json) != QUOTE) {
        return false;
    }
    return true;
}

bool IsJsonPrimitive(const JsonElement* const element) {
    small_char c = SmallStrCharAt(element->start, element->json);
    if (c == L_CRL || c == R_CRL || c == L_SQR || c == R_SQR) {
        return false;
    }
    if (IsSegmentNumber(element->json, element->start, element->end)) {
        return true;
    }
    if (IsJsonString(element)) {
        return true;
    }
    if (IsSegmentBool(element->json, element->start, element->end)) {
        return true;
    }
    if (IsSegmentNull(element->json, element->start, element->end)) {
        return true;
    }
    return false;
}

uint8_t GetPrimitiveType(const JsonElement* const element) {
    if (IsSegmentNumber(element->json, element->start, element->end)) {
        return NUMBER;
    }
    if (IsJsonString(element)) {
        return STRING;
    }
    if (IsSegmentBool(element->json, element->start, element->end)) {
        return BOOL;
    }
    if (IsSegmentNull(element->json, element->start, element->end)) {
        return NIL;
    }
    return NONE;
}

uint8_t GetElementType(const JsonElement* const element) {
    uint8_t type = GetPrimitiveType(element);
    if (type != NONE) {
        return type;
    }
    if (IsJsonObject(element)) {
        return OBJECT;
    }
    if (IsJsonArray(element)) {
        return ARRAY;
    }
    return NONE;
}

bool ValidateSmallJson(
        const small_char* const ss,
        const uint16_t len
) {
    uint16_t index = 0;
    return _ValidateValue(ss, &index, len);
}

bool _ValidateObject(
        const small_char* const json,
        uint16_t* const index,
        const uint16_t len
) {
    _NextToken(json, index, len);
    bool done = false;
    bool valid = true;
    uint16_t token;
    while(!done) {
        token = _LookAhead(json, *index, len);
        if (token == NONE) {
            valid = false;
            done = true;
        } else if (token == COMMA) {
            _NextToken(json, index, len);
        } else if (token == R_CRL) {
            _NextToken(json, index, len);
            done = true;
        } else {
            bool validKey = _ValidateString(json, index, len);
            if (!validKey) {
                valid = false;
                done = true;
            } else {
                token = _NextToken(json, index, len);
                if (token != COLON) {
                    valid = false;
                    done = true;
                } else {
                    bool validValue = _ValidateValue(json, index, len);
                    if (!validValue) {
                        valid = false;
                        done = true;
                    }
                }
            }
        }
    }
    return valid;
}

bool _ValidateArray(
        const small_char* const json,
        uint16_t* const index,
        const uint16_t len
) {
    _NextToken(json, index, len);
    bool done = false;
    bool valid = true;
    while (!done) {
        uint16_t next = _LookAhead(json, *index, len);
        if (next == NONE) {
            valid = false;
            done = true;
        } else if (next == COMMA) {
            _NextToken(json, index, len);
        } else if (next == R_SQR) {
            _NextToken(json, index, len);
            done = true;
        } else {
            bool validValue = _ValidateValue(json, index, len);
            if (!validValue) {
                valid = false;
                done = true;
            }
        }
    }
    return valid;
}

bool _ValidateValue(
        const small_char* const json,
        uint16_t* const index,
        const uint16_t len
) {
    switch (_LookAhead(json, *index, len)) {
        case STRING:
            return _ValidateString(json, index, len);
        case NUMBER:
            return _ValidateNumber(json, index, len);
        case L_CRL:
            return _ValidateObject(json, index, len);
        case L_SQR:
            return _ValidateArray(json, index, len);
        case TRUE:
            _NextToken(json, index, len);
            return true;
        case FALSE:
            _NextToken(json, index, len);
            return true;
        case NIL:
            _NextToken(json, index, len);
            return true;
        case NONE:
            break;
        default:
            break;
    }
    return false;
}

bool _ValidateString(
        const small_char* const json,
        uint16_t* const index,
        const uint16_t len
) {
    *index = (uint16_t) (*index + 1);
    uint16_t i;
    for (i = *index; i < len; i++) {
        if (SmallStrCharAt(i, json) == QUOTE) {
            break;
        }
    }
    if (i == len) {
        *index = len;
        return false;
    } else {
        *index = (uint16_t) (i + 1);
        return true;
    }
}

bool _ValidateNumber(
        const small_char* const json,
        uint16_t* const index,
        const uint16_t len
) {
    uint16_t last = _GetLastIndexOfNumber(json, *index, len);
    uint16_t cLen = (uint16_t) ((last - *index) + 1);
    small_char* subVal = (small_char*) malloc((size_t) SmallStringSize(cLen));
    SmallSubStr(subVal, json, *index, (uint16_t) (last + 1));
    bool isNumber = IsNumber(subVal, cLen);
    free(subVal);
    *index = (uint16_t) (last + 1);
    return isNumber;
}

uint16_t _GetLastIndexOfNumber(
        const small_char* const json,
        const uint16_t index,
        const uint16_t len
) {
    uint16_t last;
    for (last = index; last < len; last++) {
        small_char c = SmallStrCharAt(last, json);
        if (!IsNumeric(c) && c != PLUS && c != MINUS && c != DOT && c != E) {
            break;
        }
    }
    return (uint16_t) (last - 1);
}

small_char _LookAhead(
        const small_char* const json,
        const uint16_t index,
        const uint16_t len
) {
    uint16_t* nIndex = (uint16_t*) malloc(sizeof(uint16_t));
    *nIndex = index;
    small_char next = _NextToken(json, nIndex, len);
    free(nIndex);
    return next;
}

small_char _NextToken(
        const small_char* const json,
        uint16_t* const index,
        const uint16_t len
) {
    if (*index == len) {
        return NONE;
    }
    small_char c = SmallStrCharAt(*index, json);
    *index = (uint16_t) (*index + 1);
    switch (c) {
        case L_CRL:
            return L_CRL;
        case R_CRL:
            return R_CRL;
        case L_SQR:
            return L_SQR;
        case R_SQR:
            return R_SQR;
        case COMMA:
            return COMMA;
        case QUOTE:
            return STRING;
        case MINUS:
            return NUMBER;
        case COLON:
            return COLON;
        default:
            break;
    }
    if (IsNumeric(c)) {
        return NUMBER;
    }
    *index = (uint16_t) (*index - 1);
    uint16_t remain = len - *index;
    if (remain >= 5) {
        if (
            SmallStrCharAt(*index, json) == F &&
            SmallStrCharAt((uint16_t) (*index + 1), json) == A &&
            SmallStrCharAt((uint16_t) (*index + 2), json) == L &&
            SmallStrCharAt((uint16_t) (*index + 3), json) == S &&
            SmallStrCharAt((uint16_t) (*index + 4), json) == E
        ) {
            *index = (uint16_t) (*index + 5);
            return FALSE;
        }
    }
    if (remain >= 4) {
        if (
            SmallStrCharAt(*index, json) == T &&
            SmallStrCharAt((uint16_t) (*index + 1), json) == R &&
            SmallStrCharAt((uint16_t) (*index + 2), json) == U &&
            SmallStrCharAt((uint16_t) (*index + 3), json) == E
        ) {
            *index = (uint16_t) (*index + 4);
            return TRUE;
        }
    }
    if (remain >= 4) {
        if (
            SmallStrCharAt(*index, json) == N &&
            SmallStrCharAt((uint16_t) (*index + 1), json) == U &&
            SmallStrCharAt((uint16_t) (*index + 2), json) == L &&
            SmallStrCharAt((uint16_t) (*index + 3), json) == L
        ) {
            *index = (uint16_t) (*index + 4);
            return NIL;
        }
    }
    return NONE;
}
