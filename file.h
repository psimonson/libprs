/*
 * file.h - Library for file handling.
 *
 * Author: Philip R. Simonson
 * Date:   06/21/2024
 *
 */

#define MAXBUF 256

#define EROKAY 0
#define EROPEN 1
#define ERMEM 2
#define EWRITE 3
#define EREAD 4

struct File {
	FILE *fp;
	char name[MAXBUF];
	char data[MAXBUF];
	int length;
	size_t size;
	int err;
	char *msg;
};

void file_init(struct File *f, const char *name);
void file_open(struct File *f, int mode);
void file_close(struct File *f);
void file_read(struct File *f);
void file_write(struct File *f);
void file_printf(struct File *f, const char *msg, ...);

