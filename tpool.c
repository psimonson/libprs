/*
 * tpool.c - Source for simple thread pool implementation.
 *
 * Author: Philip R. Simonson
 * Date  : 07/31/2022
 *
 ****************************************************************************
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#if defined(__linux)
#include <unistd.h>
#endif
#include "tpool.h"

#define MAXTHREADS 8
#define MAXTASKS   32

/* Task structure. */
struct Task {
	thread_func_t func;
	void *arg;
	struct Task *next;
};

/* Simple thread pool definition. */
struct ThreadPool {
	struct Task *task_first;
	struct Task *task_last;
	pthread_mutex_t task_mutex;
	pthread_cond_t task_cond;
	pthread_cond_t working_cond;
	unsigned int ntasks;
	unsigned int nthreads;
	int stop;
};

/* --------------------------- Private Functions ------------------------- */

/* Create a task.
 */
static struct Task *tpool_task_create(thread_func_t func, void *arg)
{
	struct Task *task;

	if(func == NULL)
		return NULL;

	task = (struct Task *)malloc(sizeof(struct Task));
	if(task != NULL) {
		task->func = func;
		task->arg = arg;
		task->next = NULL;
	}
	return task;
}

/* Destroy a task.
 */
static void tpool_task_destroy(struct Task *task)
{
	if(task == NULL)
		return;
	free(task);
}

/* Get a task.
 */
static struct Task *tpool_task_get(ThreadPool *tp)
{
	struct Task *task;

	if(tp == NULL)
		return NULL;

	task = tp->task_first;
	if(task == NULL)
		return NULL;

	if(task->next == NULL) {
		tp->task_first = NULL;
		tp->task_last = NULL;
	}
	else {
		tp->task_first = task->next;
	}

	return task;
}

/* Simple thread handler.
 */
static void *tpool_worker(void *arg)
{
	ThreadPool *tp = (ThreadPool *)arg;
	struct Task *task;

	for(;;) {
		pthread_mutex_lock(&tp->task_mutex);

		while(tp->task_first == NULL && !tp->stop) {
			pthread_cond_wait(&tp->task_cond, &tp->task_mutex);
		}

		if(tp->stop) {
			break;
		}

		task = tpool_task_get(tp);
		tp->ntasks++;
		pthread_mutex_unlock(&tp->task_mutex);

		if(task != NULL) {
			task->func(task->arg);
			tpool_task_destroy(task);
		}

		pthread_mutex_lock(&tp->task_mutex);
		tp->ntasks--;
		if(!tp->stop && tp->ntasks == 0 && tp->task_first == NULL) {
			pthread_cond_signal(&tp->working_cond);
		}
		pthread_mutex_unlock(&tp->task_mutex);
	}

	tp->nthreads--;
	pthread_cond_signal(&tp->working_cond);
	pthread_mutex_unlock(&tp->task_mutex);
	return NULL;
}

/* ---------------------------- Public Functions ------------------------- */

/* Initialize empty thread pool.
 */
ThreadPool *tpool_create(unsigned int nthreads)
{
	ThreadPool *tp;
	pthread_t thread;
	unsigned int i;

	if(nthreads >= MAXTHREADS)
		nthreads = MAXTHREADS;

	if(nthreads == 0)
		nthreads = 2;

	tp = (ThreadPool *)calloc(1, sizeof(ThreadPool));
	if(tp != NULL) {
		tp->nthreads = nthreads;
		tp->task_first = NULL;
		tp->task_last = NULL;

		pthread_mutex_init(&tp->task_mutex, NULL);
		pthread_cond_init(&tp->task_cond, NULL);
		pthread_cond_init(&tp->working_cond, NULL);

		for(i = 0; i < nthreads; ++i) {
			pthread_create(&thread, NULL, tpool_worker, tp);
			pthread_detach(thread);
		}
	}
	return tp;
}

/* Add task to thread pool.
 */
int tpool_addtask(ThreadPool *tp, thread_func_t func, void *arg)
{
	struct Task *task;

	if(tp == NULL)
		return 0;

	task = tpool_task_create(func, arg);
	if(task == NULL)
		return 0;

	pthread_mutex_lock(&tp->task_mutex);
	if(tp->task_first == NULL) {
		tp->task_first = task;
		tp->task_last = tp->task_first;
	}
	else {
		tp->task_last->next = task;
		tp->task_last = task;
	}

	pthread_cond_broadcast(&tp->task_cond);
	pthread_mutex_unlock(&tp->task_mutex);
	return 1;
}

/* Wait for all tasks to stop.
 */
void tpool_wait(ThreadPool *tp)
{
	if(tp == NULL) {
		return;
	}

	pthread_mutex_lock(&tp->task_mutex);
	for(;;) {
		if((!tp->stop && tp->ntasks != 0) || (!tp->stop && tp->nthreads != 0)) {
			pthread_cond_wait(&tp->working_cond, &tp->task_mutex);
		}
		else {
			break;
		}
	}
	pthread_mutex_unlock(&tp->task_mutex);
}

/* Stop all tasks and free thread pool.
 */
void tpool_destroy(ThreadPool *tp)
{
	struct Task *task;
	struct Task *task2;

	if(tp == NULL)
		return;

	pthread_mutex_lock(&tp->task_mutex);
	task = tp->task_first;
	while(task != NULL) {
		task2 = task->next;
		tpool_task_destroy(task);
		task = task2;
	}
	tp->stop = 1;
	pthread_cond_broadcast(&tp->task_cond);
	pthread_mutex_unlock(&tp->task_mutex);

	tpool_wait(tp);

	pthread_mutex_destroy(&tp->task_mutex);
	pthread_cond_destroy(&tp->task_cond);
	pthread_cond_destroy(&tp->working_cond);
	free(tp);
}

