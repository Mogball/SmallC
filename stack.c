#include <stdlib.h>
#include "stack.h"

Stack* MakeStack(const uint16_t capacity) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (small_char*) malloc((size_t) (stack->capacity * int_size));
    return stack;
}

bool IsFull(const Stack* const stack) {
    return stack->top == stack->capacity - 1;
}

bool IsEmpty(const Stack* const stack) {
    return stack->top == -1;
}

bool Push(Stack* const stack, const small_char item) {
    if (IsFull(stack)) {
        return false;
    }
    stack->array[++stack->top] = item;
    return true;
}

small_char Pop(Stack* const stack) {
    if (IsEmpty(stack)) {
        return '\0';
    }
    return stack->array[stack->top--];
}

small_char Peek(const Stack* const stack) {
    if (IsEmpty(stack)) {
        return '\0';
    }
    return stack->array[stack->top];
}
