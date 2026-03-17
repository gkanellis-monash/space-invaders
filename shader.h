#pragma once

#include <GL/glew.h>

void validate_shader(GLuint shader, const char* file = 0);

bool validate_program(GLuint program);