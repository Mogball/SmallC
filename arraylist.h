#ifndef SMALLC_ARRAYLIST_H
#define SMALLC_ARRAYLIST_H

#include <stdint.h>
#include "structs.h"

ArrayList* MakeList(uint16_t capacity);
void DestroyList(ArrayList* list);

void _ShiftRight(ArrayList* list, uint16_t i);
void _ShiftLeft(ArrayList* list, uint16_t i);
void _EnsureCapacity(ArrayList* list);

void ListSet(ArrayList* list, uint16_t i, JsonElement e);
void ListInsert(ArrayList* list, uint16_t i, JsonElement e);
void ListAppend(ArrayList* list, JsonElement e);
JsonElement ListGet(ArrayList* list, uint16_t i);
JsonElement ListPop(ArrayList* list);
JsonElement ListRemove(ArrayList* list, uint16_t i);

#endif //SMALLC_ARRAYLIST_H
