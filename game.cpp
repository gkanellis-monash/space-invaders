#include "game.h"
#include "sprite.h"


void game_init(Game& game, size_t width, size_t height) {
    game.width = width;
    game.height = height;
    game.num_aliens = 55;
    game.aliens = new Alien[game.num_aliens];


    game.player.x = 112 - 5;
    game.player.y = 32;

    game.player.life = 3;
    game.num_bullets = 0;

    for(size_t yi = 0; yi < 5; ++yi) {
        for(size_t xi = 0; xi < 11; ++xi) {
            game.aliens[yi * 11 + xi].x = 16 * xi + 20;
            game.aliens[yi * 11 + xi].y = 17 * yi + 128;
        }
    }

}

void game_update_player(Game& game, int player_sprite_width, int player_move_dir) {
    if(game.player.x + player_sprite_width + player_move_dir >= game.width - 1) {
        game.player.x = game.width - player_sprite_width - player_move_dir - 1;
        player_move_dir *= -1;
    } else if((int)game.player.x + player_move_dir <= 0) {
        game.player.x = 0;
        player_move_dir *= -1;
    } else {
        game.player.x += player_move_dir;
    }
}


void game_update_bullet(Game& game, size_t game_height, size_t bullet_sprite_height) {
    for(size_t bi = 0; bi < game.num_bullets;) {
        game.bullets[bi].y += game.bullets[bi].dir;
        if(game.bullets[bi].y >= game_height || game.bullets[bi].y < bullet_sprite_height) {
            game.bullets[bi] = game.bullets[game.num_bullets - 1];
            --game.num_bullets;
            continue;
        }
        ++bi;
    }
}
