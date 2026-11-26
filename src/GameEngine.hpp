#ifndef DOODLE_JUMP_GAMEENGINE_HPP
#define DOODLE_JUMP_GAMEENGINE_HPP

#include "Board.hpp"
#include "TerminalManager.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class GameEngine {
private:
    Board board;
    TerminalManager terminal;

    // Потоки
    std::thread input_thread;
    std::thread compute_thread;
    std::thread render_thread;

    // Примитивы синхронизации
    std::mutex board_mutex; // Защищает доступ к объекту board
    std::atomic<bool> running{true}; // Флаг работы игры
    std::atomic<int> move_intention{0}; // -1: влево, 1: вправо, 2: выход, 0: стоп

    // Циклы для каждого потока
    void input_loop();
    void compute_loop();
    void render_loop();

public:
    GameEngine(int width, int height);
    ~GameEngine();

    // Запуск игры (блокирует главный поток до конца игры)
    void run();
};

#endif //DOODLE_JUMP_GAMEENGINE_HPP