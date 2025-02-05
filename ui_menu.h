/*
 * ui_menu.h - Header for a simple ncurses menu.
 *
 * Author: Philip R. Simonson
 * Date  : 10/22/2021
 *
 *****************************************************************************
 */

#ifndef UI_MENU_H
#define UI_MENU_H

#include <unistd.h>
#include <ncurses.h>

#ifndef UI_UTIL_H
#include "ui_util.h"
#endif

#define DEBUG 0     /* Enable this option for debugging */

/* Available default options for menu */
#ifndef NEW_MENU
OPTION_BEG
OPTION_ADD("Option 1"),
OPTION_ADD("Option 2"),
OPTION_ADD("Option 3"),
OPTION_ADD("Exit")
OPTION_END
OPTION_CNT
#else
OPTION_CNT
#endif

/* Print options for menu.
 */
static void print_menu(const char *title, int selected)
{
	int width, height, nlines;
	int i, len;

	ui_getsize(&width, &height);

	clear();
	wmake_border(NULL);

	len = strlen(title);
	nlines = 2 + noptions;

	mvprintw(height / 2 - nlines, width / 2 - len, title);
	for(i = 0; i < len; i++)
		mvaddch((height / 2 - nlines) + 1, (width / 2 - len) + i, '=');

	for(i = 0; i < noptions; i++) {
		if(i == selected)
			attron(A_REVERSE);

		mvprintw((height / 2 - nlines) + (i + 2), width / 2 - len, "%s", options[i]);

		if(i == selected)
			attroff(A_REVERSE);
	}
}

/* Custom menu for selecting options.
 */
static int ui_menu(const char *title)
{
	int c, selected = 0;
	int done = 0;

	/* For menu */
	curs_set(0);
	while(!done) {
		print_menu(title, selected);
		c = getch();

		switch(c) {
			case KEY_DOWN:
				selected++;
				if(selected >= noptions)
					selected = noptions - 1;
			break;
			case KEY_UP:
				selected--;
				if(selected <= 0)
					selected = 0;
			break;
			case 27:
			case 10:
			case KEY_ENTER:
				done = 1;
			break;
			default:
			break;
		}
	}

	/* Clear screen; display cursor on screen */
	clear();
	curs_set(1);

#if DEBUG
	if(c == 0x1B)
		printw("You've canceled the dialog.\n");
	else
		printw("Option selected was: %s\n", options[selected]);

	refresh();
	sleep(2);
#endif

	return c == 0x1B ? -1 : selected;
}

#endif
