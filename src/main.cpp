#include <iostream>
#include <ostream>
#include <unistd.h>

#include "TerminalManager.hpp"

int main() {
    TerminalManager terminal;
    terminal.clearScreen();
    bool running = true;
    while (running) {
        char ch = 0;
        ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);

        if (bytesRead > 0) {
            // something was clicked
            if (ch == 'q' || ch == 'Q') {
                running = false;
            } else if (ch == 'a' || ch == 'A') {
                // to the left
                std::cout << "\r" << std::flush;
            } else if (ch == 'd' || ch == 'D') {
                // to the right
                std::cout << "\r" << std::flush;
            }
        } else if (bytesRead == 0){
            // doing nothing, cykle goes further
        }
    }

}
