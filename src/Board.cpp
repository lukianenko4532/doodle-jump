//
// Created by vladyslav on 10.11.2026.
//

#include "Board.hpp"

#include <algorithm>
#include <random>

Board::Board(int width, int height)
    : player(width / 2.0, 2.0), width(width), height(height) {
    platforms.emplace_back(width / 2.0 - 2.0, 1.0, PlatformType::StandardStatic);
    generate_platforms_above();
}

Board::Board(Player player_in, std::vector<Platform> platforms_in, int width, int height):player(player_in), platforms(std::move(platforms_in)), width(width), height(height) {
    this->width = width;
    this->height = height;
}

void Board::generate_platforms_above() {
    double highest_y = 0.0;
    for (const auto& platform : platforms) {
        if (platform.get_y() > highest_y) {
            highest_y = platform.get_y();
        }
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<double> step_y_dist(2.0, 4.5);
    std::uniform_real_distribution<double> pos_x_dist(0.0, static_cast<double>(width - 5));
    std::uniform_int_distribution<int> type_dist(0, 99);

    while (highest_y < height) {
        highest_y += step_y_dist(gen);
        double x = pos_x_dist(gen);
        int chance = type_dist(gen);

        PlatformType type = PlatformType::StandardStatic;
        if (chance < 60) {
            type = PlatformType::StandardStatic;
        } else if (chance < 80) {
            type = PlatformType::StandardMoving;
        } else if (chance < 92) {
            type = PlatformType::FragileStatic;
        } else {
            type = PlatformType::FragileMoving;
        }

        platforms.emplace_back(x, highest_y, type);
    }
}

void Board::move_player_left() {
    player.moveLeft();
    if (player.get_x() < 0) {
        player.set_x(width - 1);
    }
}

void Board::move_player_right() {
    player.moveRight();
    if (player.get_x() >= width) {
        player.set_x(0.0);
    }
}

void Board::update() {
    if (game_over == true) {
        return;
    }
    player.update();
    for (auto& platform : platforms) {
        platform.update(width);
    }

    if (player.get_speed_y() < 0) {
        for (auto& platform : platforms) {
            if (platform.contains(player.get_x(), player.get_y()) && !platform.is_broken()) {
                player.jump();
                if (platform.get_type() == PlatformType::FragileMoving || platform.get_type() == PlatformType::FragileStatic) {
                    platform.mark_broken();
                }
                break;
            }
        }
    }

    if (player.get_y() > height / 2.0) {
        double dy = player.get_y() - (height / 2.0);
        player.shift_y(dy);
        for (auto& platform : platforms) {
            platform.shift_y(dy);
        }
        score += static_cast<int>(dy);
    }

    platforms.erase(
        std::remove_if(platforms.begin(), platforms.end(), [](const Platform& p) {
            return p.get_y() < 0 || p.is_broken();
        }),
        platforms.end()
    );

    generate_platforms_above();

    if (player.get_y() < 0) {
        game_over = true;
    }
}
