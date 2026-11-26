#include "GameEngine.hpp"
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <cmath>
#include <termios.h>

GameEngine::GameEngine(int width, int height) : board(width, height) {}

GameEngine::~GameEngine() {
    running = false;
    if (input_thread.joinable()) input_thread.join();
    if (compute_thread.joinable()) compute_thread.join();
    if (render_thread.joinable()) render_thread.join();
}

void GameEngine::run() {
    // Сброс всех случайных нажатий до старта и очистка экрана
    tcflush(STDIN_FILENO, TCIFLUSH);
    TerminalManager::clearScreen();

    // Запускаем 3 независимых потока
    input_thread = std::thread(&GameEngine::input_loop, this);
    compute_thread = std::thread(&GameEngine::compute_loop, this);
    render_thread = std::thread(&GameEngine::render_loop, this);

    // Главный поток ждёт завершения физики и рендера
    compute_thread.join();
    render_thread.join();

    // Вывод финального экрана
    std::cout << "\r\n\x1B[91mGAME OVER!\x1B[0m Final Score: " << board.get_score() << "\r\n";
}

void GameEngine::input_loop() {
    char ch = 0;
    while (running) {
        if (read(STDIN_FILENO, &ch, 1) > 0) {
            if (ch == 'a' || ch == 'A') {
                move_intention = -1;
            } else if (ch == 'd' || ch == 'D') {
                move_intention = 1;
            } else if (ch == 'q' || ch == 'Q') {
                move_intention = 2;
                running = false; // Сигнал всем потокам остановиться
            }
        }
    }
}

void GameEngine::compute_loop() {
    while (running) {
        {
            // Блокируем доступ к board на время расчётов физики
            std::lock_guard<std::mutex> lock(board_mutex);

            if (board.is_game_over()) {
                running = false;
                break;
            }

            int current_move = move_intention.exchange(0);
            if (current_move == -1) board.move_player_left();
            else if (current_move == 1) board.move_player_right();

            board.update();
        } // Мьютекс освобождается здесь

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 TPS
    }
}

void GameEngine::render_loop() {
    while (running) {
        std::string frame;
        frame.reserve(2048);

        {
            std::lock_guard<std::mutex> lock(board_mutex);

            int width = board.get_width();
            int height = board.get_height();
            std::vector<std::string> grid(height, std::string(width, ' '));

            // Отрисовка платформ (добавляем цвета через ANSI прямо в символы)
            // Но чтобы длина строки визуально не ломалась из-за скрытых кодов,
            // мы добавим цвета на этапе сборки кадра ниже.
            for (const auto& p : board.get_platforms()) {
                if (p.is_broken()) continue;
                int row = height - 1 - static_cast<int>(std::round(p.get_y()));
                int col = static_cast<int>(std::round(p.get_x()));

                if (row >= 0 && row < height) {
                    char sym = (p.get_type() == PlatformType::FragileStatic ||
                                p.get_type() == PlatformType::FragileMoving) ? '-' : '=';

                    // 'M' - Moving, 'F' - Fragile, 'S' - Standard
                    char type_marker = (p.get_type() == PlatformType::StandardMoving || p.get_type() == PlatformType::FragileMoving) ? 'M' :
                                       (sym == '-' ? 'F' : 'S');

                    for (int i = 0; i < p.get_width(); ++i) {
                        if (col + i >= 0 && col + i < width) {
                            // Хитрость: мы кладем спецсимвол в сетку, а при сборке заменим его на цветной
                            if (type_marker == 'F') grid[row][col + i] = '1'; // Хрупкая
                            else if (type_marker == 'M') grid[row][col + i] = '2'; // Движущаяся
                            else grid[row][col + i] = '3'; // Обычная
                        }
                    }
                }
            }

            // Отрисовка игрока (обозначаем символом 'P')
            const auto& player = board.get_player();
            int p_row = height - 1 - static_cast<int>(std::round(player.get_y()));
            int p_col = static_cast<int>(std::round(player.get_x()));
            if (p_row >= 0 && p_row < height && p_col >= 0 && p_col < width) {
                grid[p_row][p_col] = 'P';
            }

            // Сборка кадра с применением цветов
            frame += "\x1B[H";
            frame += "\x1B[36mScore: " + std::to_string(board.get_score()) + "\x1B[0m   [A/D - Move, Q - Quit]\r\n";
            frame += "+" + std::string(width, '-') + "+\r\n";

            for (const auto& line : grid) {
                frame += "|";
                for (char c : line) {
                    if (c == 'P') {
                        // Игрок - желтый '@'
                        frame += "\x1B[93m@\x1B[0m";
                    } else if (c == '1') {
                        // Хрупкая платформа - красная '-'
                        frame += "\x1B[91m-\x1B[0m";
                    } else if (c == '2') {
                        // Движущаяся платформа - синяя '='
                        frame += "\x1B[94m=\x1B[0m";
                    } else if (c == '3') {
                        // Обычная статичная платформа - зеленая '='
                        frame += "\x1B[92m=\x1B[0m";
                    } else {
                        frame += c;
                    }
                }
                frame += "|\r\n";
            }
            frame += "+" + std::string(width, '-') + "+\r\n";
        }

        std::cout << frame << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}