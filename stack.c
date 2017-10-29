#include <stdlib.h>
#include "stack.h"

Stack* MakeStack(const uint16_t capacity) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (uint16_t*) malloc(stack->capacity * sizeof(uint16_t));
    return stack;
}

void DestroyStack(Stack* stack) {
    free(stack->array);
    free(stack);
}

bool IsFull(const Stack* const stack) {
    return stack->top == stack->capacity - 1;
}

bool IsEmpty(const Stack* const stack) {
    return stack->top == -1;
}

bool Push(Stack* const stack, const uint16_t item) {
    if (IsFull(stack)) {
        return false;
    }
    stack->array[++stack->top] = item;
    return true;
}

uint16_t Pop(Stack* const stack) {
    if (IsEmpty(stack)) {
        return EMPTY;
    }
    return stack->array[stack->top--];
}

uint16_t Peek(const Stack* const stack) {
    if (IsEmpty(stack)) {
        return EMPTY;
    }
    return stack->array[stack->top];
}
