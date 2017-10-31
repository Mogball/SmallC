#include <stdlib.h>
#include "arraylist.h"

ArrayList* MakeList(const uint16_t capacity) {
    ArrayList* list = (ArrayList*) malloc(sizeof(ArrayList));
    list->capacity = capacity;
    list->size = 0;
    list->list = (JsonElement*) malloc(sizeof(JsonElement) * capacity);
    return list;
}
void DestroyList(ArrayList* list) {
    free(list->list);
    free(list);
}

void _ShiftRight(ArrayList* list, uint16_t i) {
    for (uint16_t p = (uint16_t) (list->size - 1); p >= i; p--) {
        list->list[p + 1] = list->list[p];
    }
}

void _ShiftLeft(ArrayList* list, uint16_t i) {
    if (i == 0) {
        i = 1;
    }
    for (uint16_t p = i; i < list->size; i++) {
        list->list[p - 1] = list->list[p];
    }
}

void _EnsureCapacity(ArrayList* list) {
    if (list->size != list->capacity) {
        return;
    }
    uint16_t n_capacity = (uint16_t) (list->capacity * 2);
    JsonElement* n_list = (JsonElement*) malloc(sizeof(JsonElement) * n_capacity);
    for (uint16_t i = 0; i < list->size; i++) {
        n_list[i] = list->list[i];
    }
    free(list->list);
    list->capacity = n_capacity;
    list->list = n_list;
}

void ListSet(ArrayList* list, uint16_t i, JsonElement e) {
    _EnsureCapacity(list);
    if (i < 0) {
        i = 0;
    }
    if (i > list->size) {
        i = list->size;
    }
    if (i == list->size) {
        list->size++;
    }
    list->list[i] = e;
}
void ListAppend(ArrayList* list, JsonElement e) {
    _EnsureCapacity(list);
    list->list[list->size] = e;
    list->size++;
}
void ListInsert(ArrayList* list, uint16_t i, JsonElement e) {
    _EnsureCapacity(list);
    _ShiftRight(list, i);
    list->list[i] = e;
}
JsonElement ListGet(ArrayList* list, uint16_t i) {
    if (i < 0) {
        i = 0;
    }
    if (i >= list->size) {
        i = (uint16_t) (list->size - 1);
    }
    return list->list[i];
}
JsonElement ListPop(ArrayList* list) {
    JsonElement e = list->list[list->size - 1];
    list->size--;
    return e;
}
JsonElement ListRemove(ArrayList* list, uint16_t i) {
    JsonElement e = list->list[i];
    _ShiftLeft(list, (uint16_t) (i + 1));
    return e;
}