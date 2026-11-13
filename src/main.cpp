#include "TerminalManager.hpp"
#include "Board.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>

int main() {
    TerminalManager terminal;
    TerminalManager::clearScreen();

    const int width = 30;
    const int height = 20;
    Board board(width, height);

    while (!board.is_game_over()) {
        // 1. Чтение ввода
        char ch = 0;
        if (read(STDIN_FILENO, &ch, 1) > 0) {
            if (ch == 'a' || ch == 'A') {
                board.move_player_left();
            } else if (ch == 'd' || ch == 'D') {
                board.move_player_right();
            } else if (ch == 'q' || ch == 'Q') {
                break;
            }
        }

        // 2. Обновление физики
        board.update();

        // 3. Подготовка буфера кадра
        std::vector<std::string> grid(height, std::string(width, ' '));

        // Отрисовка платформ
        for (const auto& p : board.get_platforms()) {
            if (p.is_broken()) continue;
            int row = height - 1 - static_cast<int>(p.get_y());
            int col = static_cast<int>(p.get_x());

            if (row >= 0 && row < height) {
                char sym = (p.get_type() == PlatformType::FragileStatic ||
                            p.get_type() == PlatformType::FragileMoving) ? '-' : '=';
                for (int i = 0; i < p.get_width(); ++i) {
                    if (col + i >= 0 && col + i < width) {
                        grid[row][col + i] = sym;
                    }
                }
            }
        }

        // Отрисовка игрока
        const auto& player = board.get_player();
        int p_row = height - 1 - static_cast<int>(player.get_y());
        int p_col = static_cast<int>(player.get_x());
        if (p_row >= 0 && p_row < height && p_col >= 0 && p_col < width) {
            grid[p_row][p_col] = player.get_symbol();
        }

        // Сборка кадра в одну строку (с использованием \r\n для raw-режима)
        std::string frame = "\x1B[H";
        frame += "Score: " + std::to_string(board.get_score()) + "   [A/D - Move, Q - Quit]\r\n";
        frame += "+" + std::string(width, '-') + "+\r\n";
        for (const auto& line : grid) {
            frame += "|" + line + "|\r\n";
        }
        frame += "+" + std::string(width, '-') + "+\r\n";

        std::cout << frame << std::flush;

        // 4. Задержка кадра
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    std::cout << "\r\n\x1B[91mGAME OVER!\x1B[0m Final Score: " << board.get_score() << "\r\n";
    return 0;
}