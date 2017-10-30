#include <stdlib.h>

#ifndef SMALLC_HASHTABLE_H
#define SMALLC_HASHTABLE_H

#define NOTHING 65535

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

HashTable* MakeTable(uint16_t size, float max_load);
void DestroyTable(HashTable* table);

void _TableRehash(HashTable* table);
uint16_t _Hash(uint16_t i);

void TablePut(HashTable* table, uint16_t key, uint16_t value);
uint16_t TableGet(const HashTable* table, uint16_t key);

#endif //SMALLC_HASHTABLE_H
