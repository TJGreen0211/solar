#ifndef RENDER_H
#define RENDER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <pthread.h>
#include <Python.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "common/shader/shader.h"
#include "common/buffer/buffer.h"
//#include "buffer/buffer.h"


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include "ui/nuklear.h"
#include "ui/nuklear_glfw_gl3.h"


typedef struct splash_screen_t {
    //int seed;
    //float radius;
    //float atmosphere_radius;
    //float semimajor_axis;
    //float orbital_period;
    //float orbital_speed;
    //float axial_tilt;
    //int has_atmosphere;
    //int lod_count;
    //int num_moons;
    //unsigned int position_buffer;
    //planet_draw_params draw_params; 
    //atmosphere_parameters atmosphere;
    //buffer sphere_faces[6];
    //simplexnoise snoise_face[6];
    //simplexnoise snoise_biomes[6];
    //buffer planet_ring;
} splash_screen_t;


typedef struct ui_componets_t {
    struct nk_colorf bg;
    struct nk_glfw glfw;
    struct nk_context *ctx;
    //ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    //{struct nk_font_atlas *atlas;
    //nk_glfw3_font_stash_begin(&glfw, &atlas);
    //nk_glfw3_font_stash_end(&glfw);
    //}
} ui_componets_t;


void *init_ui(GLFWwindow* window);

int render_menu(double delta_time, ui_componets_t *ui_menu);
void render_splash();
void render_scene(double delta_time);
void render_init_splash();

#endif