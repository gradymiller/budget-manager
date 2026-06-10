#include <ncurses.h>

void initNcurses() {
    initscr();
    noecho();
    cbreak();

    keypad(stdscr, TRUE);

    curs_set(0);

    start_color();
    use_default_colors();

    refresh();
}

void stopNcurses() {
    endwin();
}
