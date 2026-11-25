//
// Created by vladyslav on 10.11.2026.
//

#include "Player.hpp"

#include <algorithm>

constexpr double GRAVITY = 0.01;
constexpr double JUMP_FORCE = 0.4;

Player::Player(double start_x, double start_y)
    : x(start_x), y(start_y), speed_x(0.0), speed_y(0.0) {
    jump();
}
void Player::jump() {
    speed_y = JUMP_FORCE;
}

void Player::update() {
    x += speed_x;
    speed_x *= 0.87;
    if (std::abs(speed_x) < 0.05) {
        speed_x = 0;
    }
    y = y + speed_y;
    speed_y = speed_y - GRAVITY;
    speed_y = std::max(speed_y, -1.5);
}

void Player::moveLeft() {
    speed_x = -0.7;
}

void Player::moveRight() {
    speed_x = 0.7;
}

void Player::shift_y(double dy) {
    y = y - dy;
}
