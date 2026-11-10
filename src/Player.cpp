//
// Created by vladyslav on 10.11.2026.
//

#include "Player.hpp"

#include <algorithm>

constexpr double GRAVITY = 0.1;
constexpr double JUMP_FORCE = 1.2;

Player::Player(double start_x, double start_y)
    : x(start_x), y(start_y), speed_x(1.5), speed_y(0.0) {
    jump();
}

void Player::jump() {
    speed_y = JUMP_FORCE;
}

void Player::update() {
    y = y + speed_y;
    speed_y = speed_y - GRAVITY;
    speed_y = std::max(speed_y, -1.5);
}

void Player::moveLeft() {
    x -= speed_x;
}

void Player::moveRight() {
    x += speed_x;
}

void Player::shift_y(double dy) {
    y = y - dy;
}
