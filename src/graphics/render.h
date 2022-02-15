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
#include "../geometry/loader/loader.h"

//#include "ui/mainMenu.h"

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

#include "ui/asdf.h"


typedef struct main_menu_t {
    unsigned int shader;
    buffer_t object_buffer;
    struct nk_font_atlas atlas;
    struct device device;

    struct nk_font *font_14;
    struct nk_font *font_18;
    struct nk_font *font_20;
    struct nk_font *font_22;

    struct nk_image play;
    struct nk_image settings;
    struct nk_image quit;

    unsigned int exit;

} main_menu_t;

typedef struct game_scene_t {
    unsigned int shader;
    buffer_t object_buffer;
    struct nk_font_atlas atlas;
    struct device device;

    struct nk_font *font_14;
    struct nk_font *font_18;
    struct nk_font *font_20;
    struct nk_font *font_22;

    struct nk_image play;
    struct nk_image settings;
    struct nk_image quit;

    unsigned int exit;

} game_scene_t;


typedef struct splash_screen_t {
    unsigned int shader;
    buffer_t object_buffer;
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

main_menu_t *render_init_main_menu();
void render_main_menu(double delta_time, GLFWwindow* window, ui_componets_t *ui_menu, main_menu_t *main_menu);
void render_destroy_main_menu(main_menu_t *main_menu);

splash_screen_t *render_init_splash();
void render_splash(splash_screen_t *splash);
void render_destroy_splash(splash_screen_t *splash);

game_scene_t *render_init_scene();
void render_scene(double delta_time, GLFWwindow* window, ui_componets_t *ui_menu, game_scene_t *scene);
void render_destroy_scene(game_scene_t *game_menu);



#endif