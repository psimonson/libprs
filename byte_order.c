/*
 * byte_order.c - Source for checking byte order.
 *
 * Author: Philip R. Simonson
 * Date  : 07/09/2022
 *
 ****************************************************************************
 */

#include <stdio.h>
#include "byte_order.h"

/* Check for system byte order.
 */
int byte_order(void)
{
    unsigned char swaptest[4] = { 1, 2, 3, 4 };
    switch(*(unsigned int *)swaptest) {
        case 0x01020304:
            return ENDIAN_BIG;
        case 0x04010302:
            return ENDIAN_MIXED;
        case 0x04030201:
            return ENDIAN_LITTLE;
        default:
            return ENDIAN_UNKNOWN;
    }
    return ENDIAN_UNKNOWN; /* Should never get here. */
}
