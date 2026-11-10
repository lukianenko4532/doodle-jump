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
    int width, height;
    int score;
public:
    Board(Player player_in, std::vector<Platform> platforms_in, int width, int height);

    void update();

    std::vector<Platform> get_platforms() const {
        return platforms;
    }

    int get_width() const {
        return width;
    }

    int get_height() const {
        return height;
    }

    int get_score() const {
        return score;
    }
};


#endif //DOODLE_JUMP_BOARD_HPP
