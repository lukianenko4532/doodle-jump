//
// Created by vladyslav on 10.11.2026.
//

#ifndef DOODLE_JUMP_PLAYER_HPP
#define DOODLE_JUMP_PLAYER_HPP


class Player {
private:
    double x{0.0}, y{0.0}, speed_x{0.0}, speed_y{0.0};
    char symbol = '@';

public:
    Player(double start_x, double start_y);
    ~Player() = default;

    // functions for updating an image
    void update();
    void moveLeft();
    void moveRight();
    void jump();

    double get_x() const { return x; }
    double get_y() const { return y; }
    double get_speed_x() const { return speed_x; }
    double get_speed_y() const { return speed_y; }
    char get_symbol() const { return symbol; }
    void shift_y(double dy);
};


#endif //DOODLE_JUMP_PLAYER_HPP
