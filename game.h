#pragma once

#include <cstddef>

#include "alien.h"

struct Player {
    size_t x, y;
    size_t life;
};

struct Bullet {
    size_t x, y;
    int dir;
};


#define GAME_MAX_BULLETS 128
struct Game {
    size_t width, height;
    size_t num_aliens;
    Alien* aliens;
    Player player;
    Bullet bullets[GAME_MAX_BULLETS];
    size_t num_bullets;
    uint8_t* death_counters;
};

void game_init(Game& game, size_t width, size_t height);
void game_update_player(Game& game, int player_sprite_width, int player_move_dir);
void game_update_bullet(Game& game, size_t game_height, size_t bullet_sprite_height);
void game_update_aliens(Game& game);
