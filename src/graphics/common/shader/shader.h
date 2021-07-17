#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include <string.h>

unsigned int shader_create_program(char *vertex_path, char *fragment_path, char *tess_control_path, char *tess_evaluation_path, char *geometry_path);
unsigned int shader_load(char *path, int type);

#endif