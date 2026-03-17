#pragma once

#include <GL/glew.h>

extern const char* vertex_shader_source;
extern const char* fragment_shader_source;

void validate_shader(GLuint shader, const char* file = 0);

bool validate_program(GLuint program);

GLuint create_shader_program(const char* vertex_src, const char* fragment_src, GLuint& vao);