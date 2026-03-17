#pragma once

#include <cstddef>

#include "alien.h"
#include "sprite.h"

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
void game_update_player(Game& game, Sprite player_sprite, int& player_move_dir);
