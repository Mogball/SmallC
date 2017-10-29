#include "hashtable.h"

HashTable* MakeTable(const uint16_t size) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->table = (uint16_t*) malloc(sizeof(uint16_t) * size);
    table->size = size;
    for (uint16_t i = 0; i < size; i++) {
        table->table[i] = NOTHING;
    }
    return table;
}

void DestroyTable(HashTable* const table) {
    free(table->table);
    free(table);
}

void TablePut(const HashTable* const table, const uint16_t key, const uint16_t value) {
    table->table[_Hash(key)] = value;
}

uint16_t TableGet(const HashTable* const table, const uint16_t key) {
    return table->table[_Hash(key)];
}

uint16_t _Hash(uint16_t i) {
    i = (uint16_t) (((i >> 8) ^ i) * 0x45d);
    i = (uint16_t) (((i >> 8) ^ i) * 0x45d);
    i = (uint16_t) ((i >> 8) ^ i);
    return i;
}
