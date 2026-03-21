#pragma once

#include "buffer.h"

#include <cstddef>
#include <cstdint>

struct Sprite {
    size_t width, height;
    uint8_t* data;
};

struct SpriteAnimation {
    bool loop;
    size_t num_frames;
    size_t frame_duration;
    size_t time;
    Sprite** frames;
};


void buffer_sprite_draw(
    Buffer* buffer, const Sprite& sprite,
    size_t x, size_t y, uint32_t color
);


Sprite create_alien_sprite();
Sprite create_alien_animation_sprite();

Sprite create_bullet_sprite();

Sprite create_player_sprite();

SpriteAnimation* create_alien_animation(Sprite* alien_sprite0, Sprite* alien_sprite1);
void sprite_animation_update(SpriteAnimation* animation);


    