#include "pacer/terminal_display.h"
#include <ncurses.h>

TerminalDisplay::TerminalDisplay() = default;

TerminalDisplay::~TerminalDisplay() {
    shutdown();
}

void TerminalDisplay::init() {
    initscr();   // starts ncurses
    cbreak();    // disables line buffering
    noecho();    // stops showing pressed keys in terminal
    curs_set(0); // hides cursor
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE); // arrow keys
}

void TerminalDisplay::shutdown() {
    if (!isendwin()) {
        endwin();
    }
}

void TerminalDisplay::showWord(const std::string &word) {
    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);

    int row = rows / 2;
    int col = (cols - static_cast<int>(word.size())) / 2;
    if (std::max(0, col) == 0) {
        col = 0;
    }

    clear();
    mvprintw(row, col, "%s", word.c_str());
    refresh();
}

UserAction TerminalDisplay::pollInput() {
    int inp = getch();
    switch (inp) {
    case 'q':
    case 'Q':
        return UserAction::Quit;
    case ' ':
        return UserAction::Pause;
    case 'k':
    case '+':
        return UserAction::SpeedUp;
    case 'j':
    case '-':
        return UserAction::SlowDown;
    case 'h':
    case KEY_LEFT:
        return UserAction::Rewind;
    case 'l':
    case KEY_RIGHT:
        return UserAction::FastForward;
    default:
        return UserAction::None;
    }
}
