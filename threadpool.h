#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXTHREADS 10
#define MAXTASKS 300

// Structure for a task
typedef struct _task {
	void (*func)(void *);
	void *arg;
} task_t;

// Structure for a thread pool
typedef struct _threadpool {
	pthread_t threads[MAXTHREADS];
	task_t tasks[MAXTASKS];
	int front, rear, count;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int shutdown;
} threadpool_t;

// Initialize the thread pool
threadpool_t *threadpool_init(void);

// Add a task to the pool
void threadpool_add_task(threadpool_t *pool, void (*func)(void*), void *arg);

// Destroy the thread pool
void threadpool_free(threadpool_t *pool);

#endif // THREADPOOL_H

