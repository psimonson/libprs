#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ustack.h"

void
free_data (void* data)
{
    free(data);
}

int
main ()
{
    ustack_t* stack;
    char *str1,*str2,*str3;
    
    /* allocate strings */
    str1 = (char*)malloc(sizeof(char)*64);
    if(str1 == NULL) return 1;
    str2 = (char*)malloc(sizeof(char)*64);
    if(str2 == NULL) { free(str1); return 1; }
    str3 = (char*)malloc(sizeof(char)*64);
    if(str3 == NULL) { free(str1); free(str2); return 1; }

    /* fill strings */
    strcpy(str1, "Hello world!");
    strcpy(str2, "This is a test.");
    strcpy(str3, "This is another test.");
    
    /* initialize and push to stack */
    stack = create_ustack();
    if(stack == NULL) { free(str1); free(str2); free(str3); return 2; }
    push_ustack(stack, 0, str1);
    push_ustack(stack, 1, str2);
    push_ustack(stack, 2, str3);
    while(get_top_ustack(stack) > 0)
        printf("String: %s\n", (char*)pop_ustack(stack));
    reset_ustack(stack);
    destroy_ustack(&stack, free_data);
    return 0;
}
