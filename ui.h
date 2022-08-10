/*
 * ui.h - Header library for a simple user interface.
 *
 * Author: Philip R. Simonson
 * Date  : 10/22/2021
 *
 *****************************************************************************
 */

#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

typedef void (*ui_cleanup_callback)();
typedef void (*ui_init_callback)();
typedef void (*ui_loop_callback)();

/* Get the current size of the console.
 */
static void ui_getsize(int *width, int *height)
{
	int w, h;

	getmaxyx(stdscr, h, w);
	if(width != NULL) *width = w;
	if(height != NULL) *height = h;
}

/* Initialize ncurses library and setup UI.
 */
static void ui_init(ui_cleanup_callback cleanup, ui_init_callback init)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	if(init != NULL)
		init();

	if(cleanup != NULL)
		atexit(cleanup);
}

/* Run the main loop for the user interface.
 */
static int ui_loop(ui_loop_callback loop)
{
	if(loop == NULL) {
		int c, w, h;

		ui_getsize(&w, &h);
		printw("Please implement the rest of this short library...\n");
		refresh();

		while((c = getch()) != 0x1B) {
			switch(c) {
				case KEY_ENTER:
					return 0;
				default:
					/* TODO: Handle key presses */
			}
		}
	}
	else {
		loop();
	}

	return 0;
}

#endif
