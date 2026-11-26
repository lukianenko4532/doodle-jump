#ifndef DOODLE_JUMP_ARGPARSER_HPP
#define DOODLE_JUMP_ARGPARSER_HPP

#include <string>

class ArgParser {
private:
    int width{40};
    int height{20};
    bool help_requested{false};
    bool valid{true};
    std::string error_message;

public:
    ArgParser(int argc, char* argv[]);

    int get_width() const { return width; }
    int get_height() const { return height; }
    bool is_help_requested() const { return help_requested; }
    bool is_valid() const { return valid; }
    const std::string& get_error_message() const { return error_message; }

    static void print_help();
};

#endif //DOODLE_JUMP_ARGPARSER_HPP