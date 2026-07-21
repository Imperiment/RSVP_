#include "pacer/terminal_display.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <ncurses.h>
#include <string>

TerminalDisplay::TerminalDisplay() = default;

TerminalDisplay::~TerminalDisplay() {
    shutdown();
}

void TerminalDisplay::init() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);  // main word
        init_pair(2, COLOR_YELLOW, COLOR_BLACK); // paused indicator
        init_pair(3, COLOR_CYAN, COLOR_BLACK);   // status line / help
    }
}

void TerminalDisplay::shutdown() {
    if (!isendwin()) {
        endwin();
    }
}

void TerminalDisplay::showWord(const WordDisplayState &state) {
    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);

    clear();

    // --- Main word ---
    const int wordRow = rows / 2;
    int wordCol = (cols - static_cast<int>(state.word.size())) / 2;
    if (std::max(0, wordCol) == 0) {
        wordCol = 0;
    }

    if (has_colors()) {
        attron(COLOR_PAIR(1) | A_BOLD);
    }
    mvprintw(wordRow, wordCol, "%s", state.word.c_str());
    if (has_colors()) {
        attroff(COLOR_PAIR(1) | A_BOLD);
    }

    // --- Progress line ---
    const double percent = state.totalWords > 0 ? (100.0 * static_cast<double>(state.currentIndex) /
                                                   static_cast<double>(state.totalWords))
                                                : 0.0;

    std::array<char, 128> progressBuf{{}};
    std::snprintf(progressBuf.data(), progressBuf.size(), "%zu / %zu  (%.0f%%)  |  %d wpm",
                  state.currentIndex, state.totalWords, percent, state.wordsPerMinute);

    const int progressRow = wordRow + 2;
    const int progressCol =
        std::max(0, (cols - static_cast<int>(std::string(progressBuf.data()).size())) / 2);

    if (has_colors()) {
        attron(COLOR_PAIR(3));
    }
    mvprintw(progressRow, progressCol, "%s", progressBuf.data());
    if (has_colors()) {
        attroff(COLOR_PAIR(3));
    }

    // --- Paused indicator ---
    if (state.paused) {
        const char *pausedText = "-- PAUSED --";
        const int pausedCol =
            std::max(0, (cols - static_cast<int>(std::string(pausedText).size())) / 2);
        if (has_colors()) {
            attron(COLOR_PAIR(2) | A_BOLD);
        }
        mvprintw(wordRow - 2, pausedCol, "%s", pausedText);
        if (has_colors()) {
            attroff(COLOR_PAIR(2) | A_BOLD);
        }
    }

    // --- Help line ---
    const char *helpText = "[space] pause  [h/l] rewind/forward  [j/k] speed  [q] quit";
    const int helpCol = std::max(0, (cols - static_cast<int>(std::string(helpText).size())) / 2);
    if (has_colors()) {
        attron(COLOR_PAIR(3));
    }
    mvprintw(rows - 1, helpCol, "%s", helpText);
    if (has_colors()) {
        attroff(COLOR_PAIR(3));
    }

    refresh();
}

UserAction TerminalDisplay::pollInput() {
    int ch = getch();

    switch (ch) {
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
