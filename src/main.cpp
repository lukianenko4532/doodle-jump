#include "TerminalManager.hpp"
#include "Board.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <cmath>
#include <termios.h>

int main() {
    TerminalManager terminal;
    TerminalManager::clearScreen();

    // Сброс всех случайных нажатий, которые могли накопиться в буфере до старта
    tcflush(STDIN_FILENO, TCIFLUSH);

    const int width = 60;
    const int height = 40;
    Board board(width, height);

    while (!board.is_game_over()) {
        // 1. Дренаж буфера ввода (читаем всё, реагируем только на последнее намерение)
        char ch = 0;
        int move_dir = 0;
        while (read(STDIN_FILENO, &ch, 1) > 0) {
            if (ch == 'a' || ch == 'A') {
                move_dir = -1;
            } else if (ch == 'd' || ch == 'D') {
                move_dir = 1;
            } else if (ch == 'q' || ch == 'Q') {
                move_dir = 2;
            }
        }

        // Применяем управление
        if (move_dir == 1) {
            board.move_player_right();
        } else if (move_dir == -1) {
            board.move_player_left();
        } else if (move_dir == 2) {
            break;
        }

        // 2. Обновление физики
        board.update();

        // 3. Подготовка буфера кадра
        std::vector<std::string> grid(height, std::string(width, ' '));

        // Отрисовка платформ
        for (const auto& p : board.get_platforms()) {
            if (p.is_broken()) continue;
            // Используем std::round для более плавного визуального позиционирования
            int row = height - 1 - static_cast<int>(std::round(p.get_y()));
            int col = static_cast<int>(std::round(p.get_x()));

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
        int p_row = height - 1 - static_cast<int>(std::round(player.get_y()));
        int p_col = static_cast<int>(std::round(player.get_x()));
        if (p_row >= 0 && p_row < height && p_col >= 0 && p_col < width) {
            grid[p_row][p_col] = player.get_symbol();
        }

        // 4. Сборка кадра с резервированием памяти для скорости
        std::string frame;
        frame.reserve(2048); // Избегаем реаллокации в цикле

        frame += "\x1B[H"; // Перенос курсора в (1,1) без мерцания \x1B[2J
        frame += "Score: " + std::to_string(board.get_score()) + "   [A/D - Move, Q - Quit]\r\n";
        frame += "+" + std::string(width, '-') + "+\r\n";

        for (const auto& line : grid) {
            frame += "|" + line + "|\r\n";
        }
        frame += "+" + std::string(width, '-') + "+\r\n";

        // Единый вызов вывода на экран
        std::cout << frame << std::flush;

        // 5. Задержка кадра (ориентир на ~60 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "\r\n\x1B[91mGAME OVER!\x1B[0m Final Score: " << board.get_score() << "\r\n";
    return 0;
}