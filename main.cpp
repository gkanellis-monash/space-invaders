#include "buffer.h"
#include "shader.h"
#include "sprites.h"
#include "game.h"

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}


int main() {
    const size_t buffer_width = 224;
    const size_t buffer_height = 256;

    Sprite alien_sprite = create_alien_sprite();
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

    // --- DRAW INITIAL SCENE ---
    for(size_t ai = 0; ai < game.num_aliens; ++ai) {
        const Alien& alien = game.aliens[ai];
        buffer_sprite_draw(&buffer, alien_sprite,
            alien.x, alien.y, rgb_to_uint32(128, 0, 0));
    }

    buffer_sprite_draw(&buffer, player_sprite,
        game.player.x, game.player.y, rgb_to_uint32(128, 0, 0));

    // --- GAME LOOP ---
    while(!glfwWindowShouldClose(window)) {
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            buffer.width, buffer.height,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
            buffer.data
        );

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- CLEANUP ---
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
