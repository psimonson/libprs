/*
 * ctest.h - Header for simple testing framework.
 *
 * Author: Philip R. Simonson
 * Date  : 06/30/2022
 *
 ****************************************************************************
 */

#ifndef _CTEST_H_
#define _CTEST_H_

/* Forward declaration and typedef. */
struct CTest;
typedef struct CTest CTest;

/* Initialize testing framework. */
extern struct CTest *ctest_init(const char *name);

/* Destroy testing framework object. */
extern void ctest_free(struct CTest *obj);

/* Add test to array. */
extern void ctest_add(struct CTest *obj, const char *name, int (*func)(void));

/* Run all tests and report. */
extern int ctest_run(struct CTest *obj);

#endif
