#ifndef RENDER_H
#define RENDER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <pthread.h>
#include <Python.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

int render_scene(double delta_time);
void render_splash();

#endif