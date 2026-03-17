#include "shader.h"

#include <cstdio>

// --- SHADER SOURCES ---
const char* vertex_shader_source =
    "\n"
    "#version 330\n"
    "\n"
    "noperspective out vec2 TexCoord;\n"
    "\n"
    "void main(void){\n"
    "\n"
    "   TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
    "   TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
    "   \n"
    "   gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
    "}\n";

const char* fragment_shader_source =
    "\n"
    "#version 330\n"
    "\n"
    "uniform sampler2D buffer;\n"
    "noperspective in vec2 TexCoord;\n"
    "\n"
    "out vec3 outColor;\n"
    "\n"
    "void main(void) {\n"
    "    outColor = texture(buffer, TexCoord).rgb;\n"
    "}\n";

void validate_shader(GLuint shader, const char* file) {
    static const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

    if (length > 0) {
        printf("Shader %d(%s) compile error: %s\n", shader, (file? file: ""), buffer);
    }
}

bool validate_program(GLuint program) {
    static const GLsizei BUFFER_SIZE = 512;
    GLchar buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

    if (length > 0) {
        printf("Program %d link error: %s\n", program, buffer);
        return false;
    }

    return true;
}

GLuint create_shader_program(const char* vertex_src, const char* fragment_src, GLuint& vao) {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint shader_id = glCreateProgram();

    // create vertex shader
    {
        GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shader_vp, 1, &vertex_src, 0);
        glCompileShader(shader_vp);
        validate_shader(shader_vp, vertex_src);
        glAttachShader(shader_id, shader_vp);

        glDeleteShader(shader_vp);
    }

    // create fragment shader
    {
        GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shader_fp, 1, &fragment_src, 0);
        glCompileShader(shader_fp);
        validate_shader(shader_fp, fragment_src);
        glAttachShader(shader_id, shader_fp);

        glDeleteShader(shader_fp);
    }

    glLinkProgram(shader_id);

    if(!validate_program(shader_id)) {
        fprintf(stderr, "Error while validating shader.\n");
        glDeleteVertexArrays(1, &vao);
        return 0;
    }

    return shader_id;
}
