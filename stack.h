#ifndef SMALLC_STACK_H
#define SMALLC_STACK_H

#include "small_string.h"

typedef struct Stack {
    int16_t     top;
    uint16_t    capacity;
    small_char* array;
} Stack;

Stack* MakeStack(uint16_t capacity);

bool IsFull(const Stack* stack);

bool IsEmpty(const Stack* stack);

bool Push(Stack* stack, small_char item);

small_char Pop(Stack* stack);

small_char Peek(const Stack* stack);

#endif
