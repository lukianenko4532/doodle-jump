//
// Created by vladyslav on 10.11.2026.
//

#include "TerminalManager.hpp"
#include <termios.h>
#include <unistd.h>
#include <iostream>

TerminalManager::TerminalManager() {
    enableRawMode();
    hideCursor();
}

TerminalManager::~TerminalManager() {
    disableRawMode();
    showCursor();
    std::cout << "\x1B[0m" << std::flush;
}

void TerminalManager::clearScreen() {
    std::cout << "\x1B[2J\x1B[H" << std::flush;
}

void TerminalManager::enableRawMode() {
    if (isRaw_) {
        return;
    }

    tcgetattr(STDIN_FILENO, &originalTermios_);
    termios raw = originalTermios_;
    raw.c_lflag &= ~(ICANON | ECHO);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    isRaw_ = true;
}

void TerminalManager::disableRawMode() {
    if (!isRaw_) {
        return;
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios_);
    isRaw_ = false;
}

void TerminalManager::showCursor() {
    std::cout << "\x1B[?25h" << std::flush;
}

void TerminalManager::hideCursor() {
    std::cout << "\x1B[?25l" << std::flush;
}

void TerminalManager::resetCursor() {
    std::cout << "\x1B[H" << std::flush;
}



