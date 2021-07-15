#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <pthread.h>
#include <Python.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "graphics/render.h"
#include "utility/matrixmath/matrixMath.h"

typedef enum game_state {
    INIT,
	LOADING,
    MENU,
    RUNNING
} game_state;

typedef struct game_scene_t {
    game_state last_state;
    game_state current_state;
    int changed_state;

} game_scene_t;

#endif