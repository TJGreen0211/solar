#include "mainMenu.h"

void gui_init_context(GLFWwindow* window, struct nk_glfw *glfw, struct nk_context *ctx) {
    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);
    }

    //set_style(ctx, THEME_DARK);
}