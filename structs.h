#ifndef SMALLC_STRUCTS_H
#define SMALLC_STRUCTS_H

#include <stdint.h>

#define small_size 6
#define int_size 8

/**
 * Obtain the size in bytes of a small string given the
 * number of 8-bit characters it would represent. The
 * value is rounded to the nearest size of the small
 * string element defined below.
 */
#define SmallStringSize(n) (((n) * small_size + int_size - 1) / int_size)

/**
 * The element of a small string. Small string
 * 6-bit characters are inserted into an array
 * of these elements.
 */
typedef uint8_t small_char;

typedef struct JsonElement {
    const small_char* json; // backing small string
    uint16_t start;         // element start index, inclusive
    uint16_t end;           // element end index, exclusive
} JsonElement;

typedef struct Node {
    uint16_t key;
    uint16_t value;
} Node;

typedef struct HashTable {
    Node* table;
    float max_load;
    uint16_t n_nodes;
    uint16_t size;
} HashTable;

typedef struct Entry {
    JsonElement key;
    JsonElement el;
} Entry;

typedef struct HashMap {
    Entry* map;
    float max_load;
    uint16_t n_entries;
    uint16_t size;
} HashMap;

typedef struct Stack {
    int16_t     top;
    uint16_t    capacity;
    uint16_t* array;
} Stack;

typedef struct ArrayList {
    uint16_t size;
    uint16_t capacity;
    JsonElement* list;
} ArrayList;

#endif //SMALLC_STRUCTS_H
