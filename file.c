/*
 * file.c - Library for file handling.
 *
 * Author: Philip R. Simonson
 * Date:   06/21/2024
 *
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "file.h"

/* Initialise the file structure.
 */
void file_init(struct File *f, const char *name)
{
	if(f == NULL)
		return;

	strncpy(f->name, name, MAXBUF - 1);
	f->fp = NULL;
	f->data[0] = '\0';
	f->length = 0;
	f->size = 0;
	f->err = 0;
	f->msg = "OKAY";
}
/* Open the file for reading and/or writing.
 */
void file_open(struct File *f, int mode)
{
	if(mode >= 0 && mode <= 2) {
		if(mode == 0) {
			f->fp = fopen(f->name, "r");
		}
		else if(mode == 1) {
			f->fp = fopen(f->name, "w");
		}
		else if(mode == 2) {
			f->fp = fopen(f->name, "rw");
		}
		else {
			f->err = 1;
			f->msg = "FAIL";
		}
	}
	else {
		f->err = 1;
		f->msg = "FAIL";
	}
}
/* Close the given file and clear it's structure.
 */
void file_close(struct File *f)
{
	int err;

	err = fclose(f->fp);
	f->err = err;
	f->msg = err != 0 ? "FAIL" : "OKAY";
}
/* Read everything into the buffer and grow the buffer.
 */
void file_read(struct File *f)
{
	ssize_t nbytes = 0;

	if(feof(f->fp)) {
		f->data[0] = '\0';
		f->length = 0;
		f->size = 0;
		return;
	}

	nbytes = fread(f->data, 1, MAXBUF-1, f->fp);
	f->data[nbytes] = '\0';
	f->length = nbytes;
	f->size += nbytes;
}
/* Write everything into the file from the data buffer.
 */
void file_write(struct File *f)
{
	ssize_t nbytes = 0;

	if(!f->size) {
		return;
	}

	nbytes = fwrite(f->data, 1, f->length, f->fp);
	f->size -= nbytes;
}
/* Print formatted string to the file.
 */
void file_printf(struct File *f, const char *msg, ...)
{
	va_list ap;

	va_start(ap, msg);
	vfprintf(f->fp, msg, ap);
	va_end(ap);
}
