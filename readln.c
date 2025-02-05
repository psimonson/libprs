/*
 * readln.c - Read a line from file number.
 *
 * Author: Philip R. Simonson
 * Date: 5/31/2024
 *
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80
size_t readln(char **line, size_t *linecap, int fd)
{
	size_t i, size;
	FILE *fp;

	if(line == NULL) {
		return 0;
	}

	fp = fdopen(fd, "r");
	if(fp == NULL) {
		return 0;
	}

	i = 0;
	size = 0;

	*line = (char*)malloc(sizeof(char)*MAX_LINE);
	if(*line == NULL) {
		if(linecap != NULL) {
			*linecap = size;
		}
		fclose(fp);
		return 0;
	}
	size += MAX_LINE;

	for(;;) {
		short int c;

		if(i == (size - 1)) {
			char *tmp = (char*)realloc(*line, sizeof(char)*(size+MAX_LINE));
			if(tmp == NULL) {
				break;
			}
			size += MAX_LINE;
			*line = tmp;
		}

		c = fgetc(fp);
		if(c == '\n' || c == EOF) {
			(*line)[i++] = '\n';
			(*line)[i] = '\0';
			break;
		}
		else if(c == '\b' && i > 0) {
			i -= 2;
		}
		else {
			(*line)[i] = c;
		}
		++i;
	}

	fclose(fp);
	if(linecap != NULL) {
		*linecap = size;
	}
	return i;
}
#undef MAX_LINE

