// pre : include <GL/gl.h> first!
#ifndef LOADSHADER_H
#define LOADSHADER_H

GLuint load_shader(const char *path, GLenum shader_type);

GLuint load_program(const char *fragpath, const char *vertpath);

#endif
