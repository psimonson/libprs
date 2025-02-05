/*
 * clist.h - Header for a simple linked list.
 *
 * Author: Philip R. Simonson
 * Date  : 07/04/2022
 *
 ****************************************************************************
 */

#ifndef _CLIST_H_
#define _CLIST_H_

/* Declare blank enum with all types of list data. */
enum { CLIST_TYPE_UNKNOWN = -1, CLIST_TYPE_SCALAR,
       CLIST_TYPE_STRUCT, CLIST_TYPE_COUNT };

/* Declare CList structure and typedef. */
struct CList;
typedef struct CList CList;

/* Initialize the first node in the linked list. */
extern struct CList *clist_init(short int type, void *data,
    void (*free_fn)(void *data));

/* Add node to linked list. */
extern int clist_add(struct CList **head, short int type, void *data,
    void (*free_fn)(void *data));

/* Free linked list. */
extern void clist_free(struct CList *head);

/* Get the linked list's data type. */
extern short int clist_gettype(struct CList *obj);

/* Get the next link in the list. */
extern struct CList *clist_getnext(struct CList *obj);

/* Get the data from given link in the list. */
extern void *clist_getdata(struct CList *obj);

#endif
