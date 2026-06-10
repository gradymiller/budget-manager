#include <ncurses.h>

#include "ui/ncursesUtility.hpp"

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // run things

    endwin();
    return 0;
}
