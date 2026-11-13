//
// Created by vladyslav on 10.11.2026.
//

#ifndef DOODLE_JUMP_BOARD_HPP
#define DOODLE_JUMP_BOARD_HPP
#include <vector>

#include "Platform.hpp"
#include "Player.hpp"


class Board {
private:
    Player player;
    std::vector<Platform> platforms;
    bool game_over{false};
    int width{0}, height{0};
    int score{0};

    void generate_platforms_above();

public:
    Board(int width, int height);

    // Extra constructor for tests
    Board(Player player_in, std::vector<Platform> platforms_in, int width, int height);

    void update();
    void move_player_left();
    void move_player_right();

    const Player& get_player() const { return player; }
    const std::vector<Platform>& get_platforms() const { return platforms; }

    int get_width() const { return width; }
    int get_height() const { return height; }
    int get_score() const { return score; }

    bool is_game_over() const { return game_over; }
};



#endif //DOODLE_JUMP_BOARD_HPP
