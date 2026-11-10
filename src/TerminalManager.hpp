//
// Created by vladyslav on 10.11.2026.
//

#ifndef DOODLE_JUMP_TERMINALMANAGER_HPP
#define DOODLE_JUMP_TERMINALMANAGER_HPP
#pragma once

#include <termios.h>

class TerminalManager {
public:
    TerminalManager();
    ~TerminalManager();

    // Защита от дублирования управления
    TerminalManager(const TerminalManager&) = delete;
    TerminalManager& operator=(const TerminalManager&) = delete;

    // Управление режимом терминала
    void enableRawMode();
    void disableRawMode();

    // Утилиты позиционирования и курсора (ANSI)
    static void resetCursor();
    static void clearScreen();
    static void hideCursor();
    static void showCursor();

private:
    termios originalTermios_{};
    bool isRaw_{false};
};

#endif //DOODLE_JUMP_TERMINALMANAGER_HPP
