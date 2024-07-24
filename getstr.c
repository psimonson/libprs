/*
 * - getstr.c - Simple get string function.
 *
 * Author: Philip R Simonson
 * Date:   05/28/2024
 *
 ****************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Get a string from a file pointer.
 */
#define MAX_LINE 80
size_t getstr(char **line, size_t *linecap, FILE *fp)
{
	size_t i, size;

	if(line == NULL) {
		return 0;
	}

	i = 0;
	size = MAX_LINE;

	*line = (char*)malloc(sizeof(char)*size);
	if(*line == NULL) {
		return 0;
	}

	for(;;) {
		short int c;

		if(i == (size - 1)) {
			char *tmp = (char*)realloc(*line, sizeof(char)*(size + MAX_LINE));
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

	if(linecap != NULL) {
		*linecap = size;
	}
	return i;
}
#undef MAX_LINE

