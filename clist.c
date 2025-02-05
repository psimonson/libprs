/*
 * clist.c - Source for a simple linked list.
 *
 * Author: Philip R. Simonson
 * Date  : 07/04/2022
 *
 ****************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clist.h"

/* Structure for linked list. */
struct CList {
    short unsigned int type;
    short int _reserved;
    void *data;
    struct CList *next;
    void (*free)(void *data);
};

/* Initialize the linked list node.
 */
struct CList *clist_init(short int type, void *data,
    void (*free_fn)(void *data))
{
    struct CList *obj;

    obj = (struct CList *)calloc(1, sizeof(struct CList));
    if(obj != NULL) {
        obj->type = (type >= 0 && type < CLIST_TYPE_COUNT)
                    ? type : CLIST_TYPE_UNKNOWN;
        obj->data = data;
        obj->free = free_fn != NULL ? free_fn : free;
        obj->next = NULL;
    }
    return obj;
}

/* Add node to the linked list.
 */
int clist_add(struct CList **head, short int type, void *data,
    void (*free_fn)(void *data))
{
    struct CList *tmp, *node;

    node = clist_init(type, data, free_fn);
    if(node != NULL) {
		if(*head == NULL) {
			*head = node;
		} else {
        	tmp = *head;
        	while(tmp->next != NULL) {
        	    tmp = tmp->next;
        	}
        	tmp->next = node;
		}
    }
    
    return (node != NULL);
}

/* Free the linked list.
 */
void clist_free(struct CList *obj)
{
    if(obj != NULL) {
        while(obj->next != NULL) {
            struct CList *next = obj->next;
            obj->free(obj->data);
            free(obj);
            obj = next;
        }
        obj->free(obj->data);
        free(obj);
    }
}

/* Get the linked list's data type.
 */
short int clist_gettype(struct CList *obj)
{
    if(obj != NULL) {
        return obj->type;
    }
    return -1;
}

/* Get the next link in the list.
 */
struct CList *clist_getnext(struct CList *obj)
{
    if(obj != NULL) {
        return obj->next;
    }
    return NULL;
}

/* Get the linked lists data.
 */
void *clist_getdata(struct CList *obj)
{
    if(obj != NULL) {
        return obj->data;
    }
    return NULL;
}
