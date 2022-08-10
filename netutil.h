/*
 * netutil.h - Header for some network utility functions.
 *
 * Author: Philip R. Simonson
 * Date  : 06/25/2022
 *
 ****************************************************************************
 */

#ifndef _NETUTIL_H_
#define _NETUTIL_H_

#include "network.h"
#include "debug.h"

#define MAX_BUFSIZ 1024

#define STATE_QUIT 0
#define STATE_RECV 1
#define STATE_CMD  2

/* Buffer string recieved from client socket. */
extern char _nu_buffer[];
extern const char _nu_prompt[];
extern unsigned char _nu_state;

/* Recieve a string of input from client socket. */
extern int do_recv(SOCKET fd, char *buffer, long unsigned int size,
    unsigned char *state, char parse);

#endif
