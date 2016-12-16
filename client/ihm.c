#include <ncurses.h>
#include <panel.h>

WINDOW *my_wins[3]; /* une fenêtre pour le texte et une pour les utilisateurs
											 0 = fenêtre globale 1 = liste utilisateurs */
PANEL  *my_panels[3]; // panels de chaque fenêtre

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(int argc, char *argv[])
{
	int startx, starty, width, height;
	int ch;

	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	// LINES - COLS macro
	// height, width,  starty,  startx
	my_wins[0] = create_newwin(LINES-3, COLS, 0, 0);
	my_wins[1] = create_newwin(LINES, 10, 0, COLS-10);
	my_wins[2] = create_newwin(3, COLS-10, LINES-3, 0);

	int i;
	for( i = 0 ; i < 3 ; i++)
		my_panels[i] = new_panel(my_wins[i]);

	/* Update the stacking order. 2nd panel will be on top */
	update_panels();

	/* Show it on the screen */
	doupdate();
	wmove(my_wins[2],1, 1);
	refresh();

	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();	getch();
	endwin();			/* End curses mode		  */
	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners
	 * and so an ugly remnant of window.
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window
	 * 3. rs: character to be used for the right side of the window
	 * 4. ts: character to be used for the top side of the window
	 * 5. bs: character to be used for the bottom side of the window
	 * 6. tl: character to be used for the top left corner of the window
	 * 7. tr: character to be used for the top right corner of the window
	 * 8. bl: character to be used for the bottom left corner of the window
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
