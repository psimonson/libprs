#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _stack {
	void *data;
	size_t item_size;
	int capacity;
	int top;
} stack_t;

stack_t *stack_init(size_t item_size, int capacity);
void stack_push(stack_t *s, void *item);
void stack_pop(stack_t *s, void *item);
void stack_free(stack_t *s);
int stack_full(stack_t *s);
int stack_empty(stack_t *s);

#endif // STACK_H

