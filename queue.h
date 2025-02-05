#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _queue {
	void *data;
	size_t item_size;
	int front;
	int rear;
	int capacity;
} queue_t;

queue_t *queue_init(size_t item_size, int capacity);
int queue_full(queue_t *q);
int queue_empty(queue_t *q);
void queue_enqueue(queue_t *q, void *item);
void queue_dequeue(queue_t *q, void *item);
void queue_free(queue_t *q);

#endif // QUEUE_H

