#include "threadpool.h"

// Worker thread function
void *threadpool_worker(void *arg) {
	threadpool_t *pool = (threadpool_t*)arg;

	for (;;) {
		pthread_mutex_lock(&pool->mutex);

		while (pool->count == 0 && !pool->shutdown) {
			pthread_cond_wait(&pool->cond, &pool->mutex);
		}

		if (pool->shutdown) {
			pthread_mutex_unlock(&pool->mutex);
			return NULL;
		}

		task_t task = pool->tasks[pool->front];
		pool->front = (pool->front + 1) % MAXTASKS;
		pool->count--;

		pthread_mutex_unlock(&pool->mutex);

		task.func(task.arg);
	}
}

// Initialize the thread pool
threadpool_t *threadpool_init(void) {
	threadpool_t *pool = malloc(sizeof(threadpool_t));
	pool->front = pool->rear = pool->count = 0;
	pool->shutdown = 0;
	pthread_mutex_init(&pool->mutex, NULL);
	pthread_cond_init(&pool->cond, NULL);

	// Create threads
	for (int i = 0; i < MAXTHREADS; i++) {
		pthread_create(&pool->threads[i], NULL, threadpool_worker, pool);
	}
	return pool;
}

// Add a task to the pool
void threadpool_add_task(threadpool_t *pool, void (*func)(void*), void *arg) {
	pthread_mutex_lock(&pool->mutex);

	if (pool->count == MAXTASKS) {
		fprintf(stderr, "Task queue is full.\n");
		pthread_mutex_unlock(&pool->mutex);
		return;
	}

	task_t task = {func, arg};
	pool->tasks[pool->rear] = task;
	pool->rear = (pool->rear + 1) % MAXTASKS;
	pool->count++;

	pthread_cond_signal(&pool->cond);
	pthread_mutex_unlock(&pool->mutex);
}

// Destroy the thread pool
void threadpool_free(threadpool_t *pool) {
	pthread_mutex_lock(&pool->mutex);
	pool->shutdown = 1;
	pthread_cond_broadcast(&pool->cond);
	pthread_mutex_unlock(&pool->mutex);

	// Join threads
	for (int i = 0; i < MAXTHREADS; i++) {
		pthread_join(pool->threads[i], NULL);
	}

	free(pool);
}

