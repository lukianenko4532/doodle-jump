//
// Created by vladyslav on 10.11.2026.
//

#ifndef DOODLE_JUMP_PLATFORM_HPP
#define DOODLE_JUMP_PLATFORM_HPP

enum class PlatformType {
    StandardStatic,    // Standard can be used more times
    StandardMoving,
    FragileStatic,     // Fragile breaks after player touching it
    FragileMoving
};

class Platform {
private:
    double x{0.0}, y{0.0}, speed_x{0.0};
    int direction = 1; // {-1} if go to the left, {1} if go to the right
    bool broken = false;
    PlatformType type;

    static constexpr int WIDTH = 5;

public:
    Platform(double start_x, double start_y, PlatformType type = PlatformType::StandardStatic);
    ~Platform() = default;

    void update(double board_width);
    void shift_y(double dy);

    bool contains(double px, double py) const;

    double get_x() const { return x; }
    double get_y() const { return y; }
    double get_speed_x() const { return speed_x; }
    int get_width() const { return WIDTH; }
    PlatformType get_type() const { return type; }

    bool is_broken() const { return broken; }
    void mark_broken() { broken = true; }

};

#endif //DOODLE_JUMP_PLATFORM_HPP
