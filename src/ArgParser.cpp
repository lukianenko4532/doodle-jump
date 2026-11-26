#include "ArgParser.hpp"
#include <iostream>
#include <cstdlib>

ArgParser::ArgParser(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help") {
            help_requested = true;
            return;
        } else if (arg == "-w" && i + 1 < argc) {
            width = std::atoi(argv[++i]);
            if (width < 20 || width > 100) {
                valid = false;
                error_message = "Width must be between 20 and 100.";
            }
        } else if (arg == "-h" && i + 1 < argc) {
            height = std::atoi(argv[++i]);
            if (height < 10 || height > 50) {
                valid = false;
                error_message = "Height must be between 10 and 50.";
            }
        } else {
            valid = false;
            error_message = "Unknown or incomplete argument: " + arg;
        }
    }
}

void ArgParser::print_help() {
    std::cout << "\x1B[92m=== Doodle Jump CLI ===\x1B[0m\n\n";
    std::cout << "Usage: ./doodle_jump [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help       Show this help message and exit\n";
    std::cout << "  -w <width>   Set board width (20-100, default 40)\n";
    std::cout << "  -h <height>  Set board height (10-50, default 20)\n\n";
    std::cout << "Controls:\n";
    std::cout << "  [A] or [a]   Move Left\n";
    std::cout << "  [D] or [d]   Move Right\n";
    std::cout << "  [Q] or [q]   Quit Game\n";
}