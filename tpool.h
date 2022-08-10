/*
 * tpool.h - Header for simple thread pool implementation.
 *
 * Author: Philip R. Simonson
 * Date  : 07/31/2022
 *
 ****************************************************************************
 */

#ifndef _TPOOL_H_
#define _TPOOL_H_

/* Thread pool structure forward declaration and typedef. */
struct ThreadPool;
typedef struct ThreadPool ThreadPool;

/* Thread pool work function. */
typedef void (*thread_func_t)(void *arg);

/* Initialize a thread pool. */
extern ThreadPool *tpool_create(unsigned int nthreads);

/* Add task to thread pool. */
extern int tpool_addtask(ThreadPool *tp, thread_func_t func, void *data);

/* Wait for all tasks to be completed. */
extern void tpool_wait(ThreadPool *tp);

/* Destroy a thread pool. */
extern void tpool_destroy(ThreadPool *tp);

#endif
