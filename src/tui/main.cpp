#include <ncurses.h>

#include "ui/ncursesUtility.hpp"

int main() {
	// Setup up UI
	initNcurses();

	// WHILE:
	// Start START-MENU
	// IF "Start" entered, go into main budget loop
	// WHILE:
	// --- run MAIN-MENU
	// --- run operation that is selected (ncurses page?)
	// --- when "back" entered, BREAK
	// GOTO START-MENU
	// BREAK if "quit" chosen

	// Cleanup UI
	stopNcurses();
    return 0;
}
