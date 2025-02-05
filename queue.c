#include "queue.h"

queue_t *queue_init(size_t item_size, int capacity) {
	queue_t *q = malloc(sizeof(queue_t));
	if (!q) {
		return NULL;
	}
	q->data = malloc(capacity * item_size);
	if (!q->data) {
		free(q);
		return NULL;
	}
	q->item_size = item_size;
	q->front = 0;
	q->rear = -1;
	q->capacity = capacity;
	return q;
}

int queue_full(queue_t *q) {
	return (q->rear + 1) == q->capacity;
}

int queue_empty(queue_t *q) {
	return q->front == -1;
}

void queue_enqueue(queue_t *q, void *item) {
	if (queue_full(q)) {
		fprintf(stderr, "Queue is full.\n");
		return;
	}
	size_t offset = (q->rear + 1) * q->item_size;
	q->rear = (q->rear + 1) % q->capacity;
	memcpy((char*)q->data + offset, item, q->item_size);
}

void queue_dequeue(queue_t *q, void *item) {
	if (queue_empty(q)) {
		fprintf(stderr, "Queue is empty.\n");
		q->rear = 0;
		return;
	}
	memcpy(item, (char*)q->data + (q->front * q->item_size), q->item_size);
	q->front = (q->front + 1) % q->capacity;
	if (q->front == (q->rear + 1) % q->capacity) {
		q->front = -1;
		q->rear = -1;
	}
}

void queue_free(queue_t *q) {
	free(q->data);
	free(q);
}

