/*
 * ui_util.h - Header containing special functionality for different headers.
 *
 * Author: Philip R. Simonson
 * Date  : 10/23/2021
 *
 *****************************************************************************
 */

#ifndef UI_UTIL_H
#define UI_UTIL_H

#include <ctype.h>

/* Macros for dealing with custom menus */
#define OPTION_BEG static const char *options[] = {
#define OPTION_ADD(X) X
#define OPTION_END };
#define OPTION_CNT static const int noptions = sizeof(options) / sizeof(char*);

/* Make a border for the window.
 */
static void wmake_border(WINDOW *w)
{
	int y, x;
	int i, j;

	/* Set default window if NULL */
	if(w == NULL)
		w = stdscr;

	/* Get max width and height for window */
	getmaxyx(w, y, x);

	/* Draw border around window */
	for(i = 0; i < y; i++) {
		for(j = 0; j < x; j++) {
			if((i > 0 && i < (y - 1)) && (j % (x - 1)) == 0)
				mvwaddch(w, i, j, '|');
			else if((i == 0 || i == (y - 1)) && (j % (x - 1)) == 0)
				mvwaddch(w, i, j, '+');
			else if((i == 0 || i == (y - 1)) && (j % (x - 1)) != 0)
				mvwaddch(w, i, j, '-');
		}
	}

	/* Update screen */
	wrefresh(w);
}

/* Get a string from the keyboard for ncurses using window.
 */
static size_t wget_stringw(char *s, size_t sz, WINDOW *w, const char *prompt)
{
	size_t i;
	int c;

	if(w == NULL) w = stdscr;

	if(prompt != NULL) {
		wprintw(w, "%s", prompt);
		wrefresh(w);
	}

	/* Get user input from keyboard and display it */
	for(i = 0; i < (sz - 1) && (c = wgetch(w)) != EOF && c != '\n'; ) {
		if(c == KEY_BACKSPACE || c == 127) {
			if(i > 0) {
				int x, y;

				--i;
				getyx(w, y, x);
				mvwaddch(w, y, x - 1, ' ');
				wmove(w, y, x - 1);
			}
		}
		else if(c == 0x1B) {
			/* Ignore escape key */
		}
		else if(!iscntrl(c) && c < 128) {
			s[i++] = c;
			waddch(w, c);
		}
	}
	s[i] = '\0';
	return i;
}

/* Get a string from the keyboard using ncurses main window.
 */
static size_t get_stringw(char *s, size_t sz, const char *prompt)
{
	return wget_stringw(s, sz, NULL, prompt);
}

/* Create a generic password screen.
 */
static int ui_login(const char *username, const char *password)
{
	int startx, starty, width, height;
	char user[10], pass[8];
	WINDOW *login;
	int x, y, done;
	int tries;

	/* Get rid of stdscr */
	clear();
	refresh();

	/* Create window for input */
	height = 6;
	width = 30;

	/* Setup for login window */
	getmaxyx(stdscr, y, x);
	starty = (y - height) / 2;
	startx = (x - width) / 2;
	login = newwin(height, width, starty, startx);
	wrefresh(login);
	tries = 3;
	done = 0;

	while(!done && tries > 0) {
		wclear(login);
		wmake_border(login);
		mvwprintw(login, 2, 2, "Username:");
		mvwprintw(login, 3, 2, "Password:");
		wrefresh(login);

		wmove(login, 2, 12);
		wget_stringw(user, sizeof(user), login, NULL);
		wmove(login, 3, 12);
		wget_stringw(pass, sizeof(user), login, NULL);

		if(strncmp(user, username, strlen(username)) == 0
		&& strncmp(pass, password, strlen(password)) == 0)
			done = 1;
		else
			--tries;
	}

	wclear(login);
	wrefresh(login);
	delwin(login);

	return tries > 0 ? 1 : 0;
}

/* Get user input using an input box.
 */
static void ui_input(char *buf, int size, const char *title)
{
	int startx, starty, width, height;
	WINDOW *input;
	int x, y;
	int len;

	/* Setup for new window */
	getmaxyx(stdscr, y, x);
	width = 40;
	height = 3;
	startx = (x - width) / 2;
	starty = (y - height) / 2;

	/* Create the window */
	input = newwin(height, width, starty, startx);
	wclear(input);
	wmake_border(input);
	if(title != NULL) {
		int len = strlen(title);
		getmaxyx(input, y, x);
		mvwprintw(input, 0, (x - len) / 2, "%s", title);
		wrefresh(input);
	}

	/* Get string from user */
	wmove(input, 1, 1);
	do {
		len = wget_stringw(buf, size, input, NULL);
	} while(len == 0);

	/* Cleanup window */
	wclear(input);
	wrefresh(input);
	delwin(input);
}

#endif
