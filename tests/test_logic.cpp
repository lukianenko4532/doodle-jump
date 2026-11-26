#include "catch.hpp"
#include "../src/Player.hpp"
#include "../src/ArgParser.hpp"
#include "../src/Platform.hpp"

TEST_CASE("Player movement and limits", "[player]") {
    Player player(15.0, 2.0);

    SECTION("Initial jump speed is set") {
        REQUIRE(player.get_speed_y() > 0.0);
    }

    SECTION("Horizontal movement changes speed") {
        player.moveLeft();
        REQUIRE(player.get_speed_x() < 0.0);

        player.moveRight();
        REQUIRE(player.get_speed_x() > 0.0);
    }
}

TEST_CASE("ArgParser validation", "[args]") {
    SECTION("Valid arguments") {
        const char* argv[] = {"./doodle_jump", "-w", "50", "-h", "30"};
        ArgParser parser(5, const_cast<char**>(argv));

        REQUIRE(parser.is_valid() == true);
        REQUIRE(parser.get_width() == 50);
        REQUIRE(parser.get_height() == 30);
    }

    SECTION("Invalid width") {
        const char* argv[] = {"./doodle_jump", "-w", "10"}; // Меньше 20, должно выдать ошибку
        ArgParser parser(3, const_cast<char**>(argv));

        REQUIRE(parser.is_valid() == false);
    }
}

TEST_CASE("Platform collision and properties", "[platform]") {
    Platform standard_plat(10.0, 5.0, PlatformType::StandardStatic);

    SECTION("Collision detection window") {
        // Игрок падает точно на платформу
        REQUIRE(standard_plat.contains(12.0, 5.2) == true);
        // Игрок слишком высоко
        REQUIRE(standard_plat.contains(12.0, 6.0) == false);
        // Игрок промазал по X
        REQUIRE(standard_plat.contains(20.0, 5.0) == false);
    }

    SECTION("Fragile platform breaks") {
        Platform fragile_plat(10.0, 5.0, PlatformType::FragileStatic);
        REQUIRE(fragile_plat.is_broken() == false);

        fragile_plat.mark_broken();

        REQUIRE(fragile_plat.is_broken() == true);
        // Сломанная платформа больше не должна ловить игрока
        REQUIRE(fragile_plat.contains(12.0, 5.0) == false);
    }
}