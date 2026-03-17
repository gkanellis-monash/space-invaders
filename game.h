#pragma once

#include <cstddef>

#include "alien.h"

struct Player {
    size_t x, y;
    size_t life;
};

struct Game {
    size_t width, height;
    size_t num_aliens;
    Alien* aliens;
    Player player;
};

void game_init(Game& game, size_t width, size_t height);
