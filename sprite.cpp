#include "sprite.h"
#include "buffer.h"
#include "game.h"


void buffer_sprite_draw(
    Buffer* buffer, const Sprite& sprite,
    size_t x, size_t y, uint32_t color
) {
    for(size_t xi = 0; xi < sprite.width; ++xi) {
        for(size_t yi = 0; yi < sprite.height; ++yi) {
            size_t sy = sprite.height - 1 + y - yi;
            size_t sx = x + xi;
            if(
                sprite.data[yi * sprite.width + xi] &&
                sy < buffer -> height && sx < buffer -> width
            ) {
                buffer -> data[sy * buffer -> width + sx] = color;
            }
            
        }
    }

}

Sprite create_alien_sprite() {
    Sprite alien_sprite0;
    alien_sprite0.width = 11;
    alien_sprite0.height = 8;
    alien_sprite0.data = new uint8_t[11 * 8]
    {
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        0,0,0,1,0,0,0,1,0,0,0, // ...@...@...
        0,0,1,1,1,1,1,1,1,0,0, // ..@@@@@@@..
        0,1,1,0,1,1,1,0,1,1,0, // .@@.@@@.@@.
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
        1,0,1,0,0,0,0,0,1,0,1, // @.@.....@.@
        0,0,0,1,1,0,1,1,0,0,0  // ...@@.@@...
    };
    return alien_sprite0;
}

Sprite create_alien_animation_sprite() {
    Sprite alien_sprite1;
    alien_sprite1.width = 11;
    alien_sprite1.height = 8;
    alien_sprite1.data = new uint8_t[11 * 8]
    {
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
        1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
        1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
    };
    return alien_sprite1;
}

SpriteAnimation* create_alien_animation(Sprite* alien_sprite0, Sprite* alien_sprite1) {
    SpriteAnimation* alien_animation = new SpriteAnimation;

    alien_animation->loop = true;
    alien_animation->num_frames = 2;
    alien_animation->frame_duration = 10;
    alien_animation->time = 0;

    alien_animation->frames = new Sprite*[2];
    alien_animation->frames[0] = alien_sprite0;
    alien_animation->frames[1] = alien_sprite1;

    return alien_animation;
}

void sprite_animation_update(SpriteAnimation* animation) {
        ++animation->time;

        if(animation->time == animation->num_frames * animation->frame_duration) {
            if(animation->loop) {
                animation-> time = 0;
            } else {
                delete animation;
                animation = nullptr;
            }
        }
}

Sprite create_player_sprite() {
    Sprite sprite;
    sprite.width = 11;
    sprite.height = 7;
    sprite.data = new uint8_t[11 * 7]
    {
        0,0,0,0,0,1,0,0,0,0,0, // .....@.....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,0,1,1,1,0,1,1,1  // @@@.@@@.@@@
    };
    return sprite;
}


Sprite create_bullet_sprite() {
    Sprite bullet_sprite;
    bullet_sprite.width = 1;
    bullet_sprite.height = 3;
    bullet_sprite.data = new uint8_t[3]
    {
        1, // @
        1, // @
        1  // @
    };
    return bullet_sprite;
}
