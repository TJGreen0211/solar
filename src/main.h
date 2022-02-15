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
#include "utility/camera/camera.h"

typedef enum game_state_e {
    INIT,
	LOADING,
    MENU,
    RUNNING
} game_state_e;

typedef struct game_state_t {
    game_state_e last_state;
    game_state_e current_state;
    int changed_state;

} game_state_t;

#endif