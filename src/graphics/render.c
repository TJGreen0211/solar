#include "render.h"
#include "ui/nkinclude.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


void *init_ui(GLFWwindow* window) {
    ui_componets_t *ui_componets = malloc(sizeof(ui_componets_t));
    ui_componets->ctx = nk_glfw3_init(&ui_componets->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&ui_componets->glfw, &atlas);
    nk_glfw3_font_stash_end(&ui_componets->glfw);
    }

    return ui_componets;
}

buffer_t splash_buffer;

void render_init_splash() {
    int splash_shader = shader_create_program("resources/shaders/screen.vert",
        "resources/shaders/screen.frag",NULL,NULL,NULL);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

}

void render_splash() {
    glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_scene(double delta_time) {
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int render_menu(double delta_time, ui_componets_t *ui_menu) {
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
    
    return 0;
}