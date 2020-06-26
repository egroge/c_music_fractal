#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "loadshader.h"

// For infomation about the code below visit:
// https://www.khronos.org/opengl/wiki/Shader_Compilation
GLuint load_shader(const char *path, GLenum shader_type) {
    FILE *fp = fopen(path, "r");
    GLuint shader = 0;
    if(fp == NULL) {
        perror("unable to open file");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    rewind(fp);
    char *shadersrc = (char *)malloc(sizeof(char) * (len + 1));
    shadersrc[len] = '\0';
    if(fread(shadersrc, sizeof(char), len, fp) != len) {
        perror("couldn't read entire file");
        return 0;
    }
    
    shader = glCreateShader(shader_type);
    // 1 string of source code which is NULL terminated
    glShaderSource(shader, 1, (const char **) &shadersrc, NULL); 
    glCompileShader(shader);
    free(shadersrc);
    
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        GLint logsize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
        GLchar *infolog = (GLchar *) malloc(sizeof(GLchar) * logsize);      
        glGetShaderInfoLog(shader, logsize, NULL, infolog);
        fputs((const char *) infolog, stderr);
        glDeleteShader(shader);
        return 0;
    }

    return shader; 
} 


GLuint load_program(const char *fragpath, const char *vertpath) {
    GLuint fragshader = load_shader(fragpath, GL_FRAGMENT_SHADER);
    if(fragshader == 0) {
        return 0;
    }

    GLuint vertshader = load_shader(vertpath, GL_VERTEX_SHADER);
    if(vertshader == 0) {
        glDeleteShader(fragshader);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, fragshader);
    glAttachShader(program, vertshader);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        GLint logsize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);
        GLchar *infolog = (GLchar *) malloc(sizeof(GLchar) * logsize);      
        glGetProgramInfoLog(program, logsize, NULL, infolog);
        fputs((const char *) infolog, stderr);   
    }

    glDetachShader(program, fragshader);
    glDetachShader(program, vertshader);

    glDeleteShader(fragshader);
    glDeleteShader(vertshader);

    return program;
}
