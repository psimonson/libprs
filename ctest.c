/*
 * ctest.c - Source for a simple testing framework implementation.
 *
 * Author: Philip R. Simonson
 * Date  : 06/30/2022
 *
 ****************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#define CTEST_MAXNAME 128
#define CTEST_MAXTEST 64

#define MIN(A, B) (((A) < (B)) ? (A) : (B))

/* Structure for the framework. */
struct CTest {
    char name[CTEST_MAXNAME];
    short int error;
    short unsigned int ntest;
    short unsigned int npass;
    short int _reserved;
    struct {
        char name[CTEST_MAXNAME];
        int (*func)(void);
    } tests[CTEST_MAXTEST];
};

/* Initialize testing framework.
 */
struct CTest *ctest_init(const char *name)
{
    struct CTest *obj;

    obj = (struct CTest *)calloc(1, sizeof(struct CTest));
    if(obj != NULL) {
        strncpy(obj->name, name, CTEST_MAXNAME-1);
    }
    return obj;
}

/* Destroy testing framework object.
 */
void ctest_free(struct CTest *obj)
{
    if(obj != NULL) {
        free(obj);
    }
}

/* Add test to array.
 */
void ctest_add(struct CTest *obj, const char *name, int (*func)(void))
{
    if(obj != NULL) {
        if(obj->ntest < CTEST_MAXTEST) {
            int len = strlen(name);

            len = MIN(len, CTEST_MAXNAME-1);
            strncpy(obj->tests[obj->ntest].name, name, len);
            obj->tests[obj->ntest].func = func;
            obj->ntest++;
        }
        DEBUG_WARNING(obj->ntest < CTEST_MAXTEST,
                      "Cannot add another tests, full.");
    }
}

/* Run all tests and report.
 */
int ctest_run(struct CTest *obj)
{
    int i;

    if(obj == NULL) return -1;

    printf("Starting tests [%s]...\n", obj->name);

    /* Run all tests. */
    for(i = 0; i < obj->ntest; i++) {
        int res;

        printf(" - Running test: %-40s ", obj->tests[i].name);
        res = obj->tests[i].func() == 0;
        printf("[%s]\n", res == 0 ? "PASS" : "FAIL");
        obj->npass += (res == 0);
    }

    /* Print results. */
    printf("Tests for [%s] finished!\nResults below...\n"
           "Total number of tests passed: %hu/%hu.\n",
            obj->name, obj->npass, obj->ntest);

    return !(obj->ntest == obj->npass);
}
