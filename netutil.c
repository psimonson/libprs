/*
 * netutil.c - Source for some utility functions for networking.
 *
 * Author: Philip R. Simonson
 * Date  : 06/25/2022
 *
 ****************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "netutil.h"

const char _nu_prompt[] = "> ";
static long unsigned int pos;
char _nu_buffer[MAX_BUFSIZ];
unsigned char _nu_state;

/* Parse the given string. */
extern void parse(SOCKET fd, char *buffer);

int do_recv(SOCKET fd, char *buffer, long unsigned int size,
    unsigned char *state, char will_parse)
{
    int nbytes;
    char buf[32];

    pos = 0;
    *state = STATE_RECV;
    while(*state != STATE_QUIT) {
        switch(*state) {
            case STATE_RECV:
                /* Recieve from client, if client closed disconnect. */
                nbytes = recv(fd, buf, 1, 0);
                if(nbytes > 0) {
                    buf[nbytes] = 0;
                }
                else {
                    *state = STATE_QUIT;
                }

                /* Append to buffer. */
                if(pos < size-1) {
                    buffer[pos++] = buf[0];
                    buffer[pos] = 0;
                }

                /* Check against available commands. */
                if(buf[0] == '\r' || buf[0] == '\n') {
                    *state = STATE_CMD;
                    pos = 0;
                }
            break;
            case STATE_CMD:
                if(will_parse) {
                    *state = STATE_RECV;
                    parse(fd, buffer);
                    if(*state != STATE_QUIT) {
                        send(fd, _nu_prompt, strlen(_nu_prompt), 0);
                    }
                }
                else {
                    *state = STATE_QUIT;
                }
            break;
            default:
                /* Do nothing */
            break;
        }
    }
    return nbytes;
}
