#include "stack.h"

stack_t *stack_init(size_t item_size, int capacity) {
	stack_t *s = (stack_t*)malloc(sizeof(stack_t));
	if (!s) {
		return NULL;
	}
	s->data = malloc(capacity * item_size);
	if (!s->data) {
		free(s);
		return NULL;
	}
	s->item_size = item_size;
	s->capacity = capacity;
	s->top = -1;
	return s;
}

void stack_push(stack_t *s, void *item) {
	if (stack_full(s)) {
		fprintf(stderr, "Stack is full.\n");
		return;
	}
	memcpy((char*)s->data + ((s->top + 1) * s->item_size), item, s->item_size);
	s->top = (s->top + 1) % s->capacity;
}

void stack_pop(stack_t *s, void *item) {
	if (stack_empty(s)) {
		fprintf(stderr, "Stack is empty.\n");
		return;
	}
	memcpy((char*)item, s->data + (s->top * s->item_size), s->item_size);
	s->top = (s->top - 1) % s->capacity;
}

void stack_free(stack_t *s) {
	free(s->data);
	free(s);
}

int stack_full(stack_t *s) {
	return (s->top + 1) == s->capacity;
}

int stack_empty(stack_t *s) {
	return s->top == -1;
}

