/*
 * byte_order.h - Header for simple byte order checking.
 *
 * Author: Philip R. Simonson
 * Date  : 07/09/2022
 *
 ****************************************************************************
 */

#ifndef _BYTE_ORDER_H_
#define _BYTE_ORDER_H_

#define BYTE_ORDER byte_order()

enum { ENDIAN_BIG, ENDIAN_LITTLE, ENDIAN_MIXED, ENDIAN_UNKNOWN };

/* Check byte order of system. */
extern int byte_order(void);

#endif
