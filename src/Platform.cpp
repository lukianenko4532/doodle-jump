//
// Created by vladyslav on 10.11.2026.
//

#include "Platform.hpp"

Platform::Platform(double start_x, double start_y, PlatformType type)
    : x(start_x), y(start_y), type(type) {

    if (type == PlatformType::StandardMoving || type == PlatformType::FragileMoving) {
        speed_x = 0.4;
    } else {
        speed_x = 0.0;
    }
}

void Platform::update(double board_width) {
    if (type != PlatformType::StandardMoving && type != PlatformType::FragileMoving) {
        return;
    }

    x += speed_x * direction;

    if (x <= 0) {
        direction = 1;
        x = 0;
    }

    if ( x + WIDTH >= board_width) {
        direction = -1;
        x = board_width - WIDTH;
    }
}

bool Platform::contains(double px, double py) const {
    if (broken) {
        return false;
    }
    return (px >= x && px <= x + WIDTH && py <= y + 0.3 && py >= y - 1.0);
}

void Platform::shift_y(double dy) {
    y = y - dy;
}
