#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun);


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

auto main() -> int {
    // std::cout << "Hi\n";

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window;

    if(!glfwInit()) {
        return -1;
    }

    // give ""hints" about GLFW versioning
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    /* Create a windowed mode window and its OpenGL context */
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

    int glVersion[2] = {-1, 1};
    glGetIntegerv(GLFW_VERSION_MAJOR, &glVersion[0]);
    glGetIntegerv(GLFW_VERSION_MINOR, &glVersion[1]);

    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}
