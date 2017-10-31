#include <stdlib.h>
#include "structs.h"

#ifndef SMALLC_HASHTABLE_H
#define SMALLC_HASHTABLE_H

#define NOTHING 65535

HashTable* MakeTable(uint16_t size, float max_load);
void DestroyTable(HashTable* table);

void _TableRehash(HashTable* table);
uint16_t _Hash(uint16_t i);

void TablePut(HashTable* table, uint16_t key, uint16_t value);
uint16_t TableGet(const HashTable* table, uint16_t key);

HashMap* MakeMap(uint16_t size, float max_load);
void DestroyMap(HashMap* map);

void _MapRehash(HashMap* map);
uint32_t _MapHash(JsonElement key);

void MapPut(HashMap* map, JsonElement key, JsonElement el);
JsonElement MapGet(const HashMap* map, JsonElement key);

uint16_t MapLength(HashMap* map);

#endif //SMALLC_HASHTABLE_H
