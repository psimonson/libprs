/*
 * clog - Simple program logging system.
 *
 * Author: Philip R. Simonson
 * Date:   06/22/2024
 *
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "clog.h"
#include "file.h"

/* Initialise the log file structure.
 */
void log_init(struct Log *l, const char *name)
{
	if(l == NULL || name == NULL) {
		return;
	}

	l->fp = NULL;
	strncpy(l->name, name, MAXBUF-1);
	l->data[0] = '\0';
	l->length = 0;
	l->size = 0;
	l->err = 0;
	l->msg = "";
}
/* Open a log file for reading, writing, and/or appending.
 */
void log_open(struct Log *l, int mode)
{
	if(l == NULL) {
		l->err = 1;
		l->msg = ERROPEN;
		return;
	}

	switch(mode) {
		case 0:
			l->fp = fopen(l->name, "r");
			break;
		case 1:
			l->fp = fopen(l->name, "w");
			break;
		case 2:
			l->fp = fopen(l->name, "a");
			break;
		case 3:
			l->fp = fopen(l->name, "rw");
			break;
		default:
			l->err = 1;
			l->msg = ERROPEN;
			return;
	}

	/* Sanity check. */
	if(l->fp == NULL) {
		l->err = 1;
		l->msg = ERROPEN;
		return;
	}

	l->err = 0;
}
/* Close a log file that is open.
 */
void log_close(struct Log *l)
{
	l->err = fclose(l->fp);
	l->msg = l->err != 0 ? "Error: Close failed" : "Closed file";
}
/* Read contents of a log file into the buffer.
 */
void log_read(struct Log *l)
{
	ssize_t nbytes = 0;

	if(l == NULL) {
		l->err = 2;
		l->msg = ERRREAD;
		return;
	}

	nbytes = fread(l->data, 1, MAXBUF-1, l->fp);
	l->length = nbytes;

	if(!l->size) {
		l->err = 0;
		l->msg = "Read complete";
		return;
	}
	else {
		l->size -= nbytes;
	}
}
/* Print formatted string into log file.
 */
void log_printf(struct Log *l, const char *msg, ...)
{
	va_list ap;

	va_start(ap, msg);
	vfprintf(l->fp, msg, ap);
	va_end(ap);
}
