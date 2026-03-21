#include "buffer.h"
#include "shader.h"
#include "sprite.h"
#include "game.h"

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


int move_dir;
bool game_running = false;
bool fire_pressed = false;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch(key) {
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS) game_running = false;
            break;
        default:
            break;
    case GLFW_KEY_RIGHT:
        if(action == GLFW_PRESS) move_dir += 1;
        else if(action == GLFW_RELEASE) move_dir -= 1;
        break;
    case GLFW_KEY_LEFT:
        if(action == GLFW_PRESS) move_dir -= 1;
        else if(action == GLFW_RELEASE) move_dir += 1;
        break;
    case GLFW_KEY_SPACE:
        if(action == GLFW_RELEASE) fire_pressed = true;
        break;
    }
}


int main() {
    game_running = true;

    const size_t buffer_width = 224;
    const size_t buffer_height = 256;

    Sprite alien_sprite0 = create_alien_sprite();
    Sprite alien_sprite1 = create_alien_animation_sprite();

    Sprite bullet_sprite = create_bullet_sprite();

    Sprite player_sprite = create_player_sprite();

    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if(!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(640, 480, "Space Invaders", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if(err != GLEW_OK) {
        fprintf(stderr, "Error initialising GLEW.\n");
        glfwTerminate();
        return -1;
    }

    int glVersion[2] = {-1, -1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);

    // --- BUFFER SETUP ---
    uint32_t clear_color = rgb_to_uint32(0, 128, 0);
    Buffer buffer;
    buffer.width  = buffer_width;
    buffer.height = buffer_height;
    buffer.data   = new uint32_t[buffer.width * buffer.height];
    buffer_clear(&buffer, clear_color);

    // --- GAME SETUP ---
    Game game;
    game_init(game, buffer.width, buffer.height);

    // --- SHADER + VAO SETUP ---
    GLuint fullscreen_triangle_vao;
    GLuint shader_id = create_shader_program(
        vertex_shader_source,
        fragment_shader_source,
        fullscreen_triangle_vao
    );
    if(!shader_id) {
        glfwTerminate();
        delete[] buffer.data;
        return -1;
    }

    glUseProgram(shader_id);

    // --- TEXTURE SETUP ---
    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);

    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB8,
        buffer.width, buffer.height, 0,
        GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLint location = glGetUniformLocation(shader_id, "buffer");
    glUniform1i(location, 0);

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(fullscreen_triangle_vao);

    // --- ANIMATION SETUP ---
    glfwSwapInterval(1);
    SpriteAnimation* alien_animation = create_alien_animation(&alien_sprite0, &alien_sprite1);


    // --- GAME LOOP ---
    while(!glfwWindowShouldClose(window) && game_running) {

        buffer_clear(&buffer, clear_color);
        
        // DRAW PLAYER
        game_update_player(game, player_sprite.width, move_dir);
        buffer_sprite_draw(
            &buffer, 
            player_sprite,
            game.player.x, 
            game.player.y, rgb_to_uint32(128, 0, 0)
        );

        // DRAW ALIENS
        sprite_animation_update(alien_animation);
        for(size_t ai =0; ai < game.num_aliens; ++ai) {
            const Alien& alien = game.aliens[ai];
            size_t current_frame = alien_animation->time / alien_animation->frame_duration;
            const Sprite& sprite = *alien_animation->frames[current_frame];
            buffer_sprite_draw(
                &buffer, sprite, 
                alien.x, alien.y, 
                rgb_to_uint32(128, 0, 0)
            );
        }

        // DRAW BULLETS
        for(size_t bi = 0; bi < game.num_bullets; ++bi) {
            buffer_sprite_draw(
                &buffer, bullet_sprite,
                game.bullets[bi].x, game.bullets[bi].y, 
                rgb_to_uint32(128, 0, 0)
            );
        }
        game_update_bullet(game, buffer.height, bullet_sprite.height);
        
        // UPLOAD TO GPU AND PRESENT
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            buffer.width, buffer.height,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
            buffer.data
        );

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(fire_pressed && game.num_bullets < GAME_MAX_BULLETS) {
            game.bullets[game.num_bullets].x = game.player.x + player_sprite.width / 2;
            game.bullets[game.num_bullets].y = game.player.y + player_sprite.height;
            game.bullets[game.num_bullets].dir = 2;
            ++game.num_bullets;
        }
        fire_pressed = false;
    }



    // --- CLEANUP ---
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
