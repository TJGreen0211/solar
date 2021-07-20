#include "render.h"
#include "ui/nkinclude.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

static struct nk_image image_load_icon(const char *filename) {
    int x,y,n;
    unsigned int tex;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    if (!data) {
        printf("[SDL]: failed to load image: %s", filename);
    }
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return nk_image_id((int)tex);
}


void *init_ui(GLFWwindow* window) {
    ui_componets_t *ui_componets = malloc(sizeof(ui_componets_t));
    ui_componets->ctx = nk_glfw3_init(&ui_componets->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&ui_componets->glfw, &atlas);
    nk_glfw3_font_stash_end(&ui_componets->glfw);
    }

    return ui_componets;
}


splash_screen_t *render_init_splash() {
    splash_screen_t *splash = malloc(sizeof(splash_screen_t));
    buffer_t object_buffer = {0};

    splash->shader = shader_create_program("resources/shaders/screen.vert",
        "resources/shaders/screen.frag",NULL,NULL,NULL);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    object_buffer.point_size = sizeof(vertices);
    
    object_buffer.points = malloc(sizeof(float)*object_buffer.point_size/sizeof(vertices[0]));
    memcpy(object_buffer.points, vertices, sizeof(float)*object_buffer.point_size/sizeof(vertices[0]));

    buffer_init_object(&object_buffer);
    
    object_buffer.vertex_number = (object_buffer.point_size/sizeof(vertices[0]))/3;

    splash->object_buffer = object_buffer;
    printf("VERTEX NUMBER: %d\n", splash->object_buffer.vertex_number);

    return splash;
}

void render_splash(splash_screen_t *splash) {
    glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(splash->shader);
    glBindVertexArray(splash->object_buffer.vao);
    glDrawArrays(GL_TRIANGLES, 0, splash->object_buffer.vertex_number);
}

void render_destroy_splash(splash_screen_t *splash) {
    free(splash);
}

void render_scene(double delta_time) {
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

main_menu_t *render_init_main_menu() {
    main_menu_t *main_menu = malloc(sizeof(main_menu_t));

    main_menu->play = image_load_icon("resources/ui/main_menu/icons/play.png");
    main_menu->settings = image_load_icon("resources/ui/main_menu/icons/play.png");
    main_menu->quit = image_load_icon("resources/ui/main_menu/icons/play.png");
    return main_menu;
}

void render_main_menu(double delta_time, ui_componets_t *ui_menu, main_menu_t *main_menu) {
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    nk_glfw3_new_frame(&ui_menu->glfw);
    if (nk_begin(ui_menu->ctx, "Demo", nk_rect(50, 50, 230, 250),
    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
                
        enum {EASY, HARD};
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(ui_menu->ctx, 30, 80, 1);
        if (nk_button_label(ui_menu->ctx, "button"))
            fprintf(stdout, "button pressed\n");

        //static const float ratio[] = {0.15f, 0.85f};
        //nk_style_set_font(ui_menu->ctx, &main_menu->font_22->handle);
        //nk_layout_row(ui_menu->ctx, NK_DYNAMIC, height, 2, ratio);
        //nk_spacing(ui_menu->ctx, 1);
        if (nk_button_image_label(ui_menu->ctx, main_menu->play, "Styled", NK_TEXT_CENTERED))
            fprintf(stdout, "rocket!\n");

        nk_layout_row_dynamic(ui_menu->ctx, 30, 2);
        if (nk_option_label(ui_menu->ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(ui_menu->ctx, "hard", op == HARD)) op = HARD;

        nk_layout_row_dynamic(ui_menu->ctx, 25, 1);
        nk_property_int(ui_menu->ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(ui_menu->ctx, 20, 1);
        nk_label(ui_menu->ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ui_menu->ctx, 25, 1);
        if (nk_combo_begin_color(ui_menu->ctx, nk_rgb_cf(ui_menu->bg), nk_vec2(nk_widget_width(ui_menu->ctx),400))) {
            nk_layout_row_dynamic(ui_menu->ctx, 120, 1);
            ui_menu->bg = nk_color_picker(ui_menu->ctx, ui_menu->bg, NK_RGBA);
            nk_layout_row_dynamic(ui_menu->ctx, 25, 1);
            ui_menu->bg.r = nk_propertyf(ui_menu->ctx, "#R:", 0, ui_menu->bg.r, 1.0f, 0.01f,0.005f);
            ui_menu->bg.g = nk_propertyf(ui_menu->ctx, "#G:", 0, ui_menu->bg.g, 1.0f, 0.01f,0.005f);
            ui_menu->bg.b = nk_propertyf(ui_menu->ctx, "#B:", 0, ui_menu->bg.b, 1.0f, 0.01f,0.005f);
            ui_menu->bg.a = nk_propertyf(ui_menu->ctx, "#A:", 0, ui_menu->bg.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(ui_menu->ctx);
        }
    }
    nk_end(ui_menu->ctx);
    nk_glfw3_render(&ui_menu->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}


void render_destroy_main_menu(main_menu_t *main_menu) {
    free(main_menu);
}