#include "GameEngine.hpp"
#include "ArgParser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    ArgParser parser(argc, argv);

    // Если запросили помощь, выводим её и выходим
    if (parser.is_help_requested()) {
        ArgParser::print_help();
        return 0;
    }

    // Если ввели что-то не то, выводим ошибку
    if (!parser.is_valid()) {
        std::cerr << "\x1B[91mError: " << parser.get_error_message() << "\x1B[0m\n";
        std::cerr << "Run with --help for usage information.\n";
        return 1;
    }

    // Запускаем игру с параметрами из консоли (или дефолтными 40x20)
    GameEngine game(parser.get_width(), parser.get_height());
    game.run();

    return 0;
}