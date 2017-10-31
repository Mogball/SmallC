#ifndef SMALLC_STACK_H
#define SMALLC_STACK_H

#include <stdbool.h>
#include "structs.h"

#define EMPTY 0xff

Stack* MakeStack(uint16_t capacity);

void DestroyStack(Stack* stack);

bool IsFull(const Stack* stack);

bool IsEmpty(const Stack* stack);

bool Push(Stack* stack, uint16_t item);

uint16_t Pop(Stack* stack);

uint16_t Peek(const Stack* stack);

#endif
