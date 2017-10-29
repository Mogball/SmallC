#include <stdlib.h>

#ifndef SMALLC_HASHTABLE_H
#define SMALLC_HASHTABLE_H

#define NOTHING 65535

typedef struct HashTable {
    uint16_t* table;
    uint16_t size;
} HashTable;

HashTable* MakeTable(uint16_t size);
void DestroyTable(HashTable* table);

uint16_t _Hash(uint16_t i);

void TablePut(const HashTable* table, uint16_t key, uint16_t value);
uint16_t TableGet(const HashTable* table, uint16_t key);

#endif //SMALLC_HASHTABLE_H
