#include "hashtable.h"

HashTable* MakeTable(const uint16_t size, const float max_load) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->table = (Node*) malloc(sizeof(Node) * size);
    table->size = size;
    table->max_load = max_load;
    table->n_nodes = 0;
    Node empty;
    empty.key = NOTHING;
    empty.value = NOTHING;
    for (uint16_t i = 0; i < size; i++) {
        table->table[i] = empty;
    }
    return table;
}

void DestroyTable(HashTable* const table) {
    free(table->table);
    free(table);
}

void _TableRehash(HashTable* const table) {
    HashTable* n_table = MakeTable((const uint16_t) (table->size * 2), table->max_load);
    for (uint16_t i = 0; i < table->size; i++) {
        Node node = table->table[i];
        if (node.value != NOTHING) {
            TablePut(n_table, node.key, node.value);
        }
    }
    free(table->table);
    table->table = n_table->table;
    table->size = n_table->size;
    free(n_table);
}

void TablePut(HashTable* const table, const uint16_t key, const uint16_t value) {
    uint16_t hash = _Hash(key) % table->size;
    while(table->table[hash].value != NOTHING) {
        hash++;
        if (hash >= table->size) {
            hash = 0;
        }
    }
    Node node;
    node.key = key;
    node.value = value;
    table->n_nodes++;
    table->table[hash] = node;
    if (table->n_nodes > table->max_load * table->size) {
        _TableRehash(table);
    }
}

uint16_t TableGet(const HashTable* const table, const uint16_t key) {
    uint16_t hash = _Hash(key) % table->size;
    while(table->table[hash].key != key) {
        hash++;
        if (hash >= table->size) {
            hash = 0;
        }
    }
    return table->table[hash].value;
}

uint16_t _Hash(uint16_t i) {
    unsigned int x = i;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return (uint16_t) x;
}
