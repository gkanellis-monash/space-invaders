#include "game.h"


void game_init(Game& game, size_t width, size_t height) {
    game.width = width;
    game.height = height;
    game.num_aliens = 55;
    game.aliens = new Alien[game.num_aliens];

    game.player.x = 112 - 5;
    game.player.y = 32;

    game.player.life = 3;

    for(size_t yi = 0; yi < 5; ++yi) {
        for(size_t xi = 0; xi < 11; ++xi) {
            game.aliens[yi * 11 + xi].x = 16 * xi + 20;
            game.aliens[yi * 11 + xi].y = 17 * yi + 128;
        }
    }

}