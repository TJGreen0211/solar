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

static void device_upload_atlas(struct device *dev, const void *image, int width, int height)
{
    glGenTextures(1, &dev->font_tex);
    glBindTexture(GL_TEXTURE_2D, dev->font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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

    geometry mesh;
    geometry_load_obj_file("resources/models/ship.obj", &mesh);
    // p->mesh_buffer = buffer_init(mesh);
    geometry_mesh_dealloc(&mesh);

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

game_scene_t *render_init_scene() {
    game_scene_t *scene = malloc(sizeof(game_scene_t));
    buffer_t object_buffer = {0};

    scene->shader = shader_create_program("resources/shaders/screen.vert",
        "resources/shaders/screen.frag",NULL,NULL,NULL);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    printf("Initializing scene\n");

    object_buffer.point_size = sizeof(vertices);
    
    object_buffer.points = malloc(sizeof(float)*object_buffer.point_size/sizeof(vertices[0]));
    memcpy(object_buffer.points, vertices, sizeof(float)*object_buffer.point_size/sizeof(vertices[0]));

    buffer_init_object(&object_buffer);
    
    object_buffer.vertex_number = (object_buffer.point_size/sizeof(vertices[0]))/3;

    scene->object_buffer = object_buffer;

    //mainmenu_device_init(&main_menu->device);
    
    {const void *image; int w, h;
    struct nk_font_config cfg = nk_font_config(0);
    cfg.oversample_h = 3; cfg.oversample_v = 2;
    nk_font_atlas_init_default(&scene->atlas);
    nk_font_atlas_begin(&scene->atlas);
    scene->font_14 = nk_font_atlas_add_from_file(&scene->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 14.0f, &cfg);
    scene->font_18 = nk_font_atlas_add_from_file(&scene->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 18.0f, &cfg);
    scene->font_20 = nk_font_atlas_add_from_file(&scene->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 20.0f, &cfg);
    scene->font_22 = nk_font_atlas_add_from_file(&scene->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 22.0f, &cfg);
    image = nk_font_atlas_bake(&scene->atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(&scene->device, image, w, h);
    nk_font_atlas_end(&scene->atlas, nk_handle_id((int)scene->device.font_tex), &scene->device.null);}
    
    scene->play = image_load_icon("resources/ui/main_menu/icons/play.png");
    scene->settings = image_load_icon("resources/ui/main_menu/icons/play.png");
    scene->quit = image_load_icon("resources/ui/main_menu/icons/play.png");

    scene->exit = 0;


    return scene;
}

void render_scene(double delta_time, GLFWwindow* window, ui_componets_t *ui_menu, game_scene_t *scene) {
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(scene->shader);
    glBindVertexArray(scene->object_buffer.vao);
    glDrawArrays(GL_TRIANGLES, 0, scene->object_buffer.vertex_number);

    nk_glfw3_new_frame(&ui_menu->glfw);
    nk_style_set_font(ui_menu->ctx, &scene->font_20->handle);
    if (nk_begin(ui_menu->ctx, "Demo", nk_rect(50, 50, 250, 600),
    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
                
        //nk_layout_row_static(ui_menu->ctx, 30, 80, 1);


        static const float ratio[] = {0.15f, 0.85f};
        nk_style_set_font(ui_menu->ctx, &scene->font_22->handle);
        nk_layout_row(ui_menu->ctx, NK_DYNAMIC, 50, 2, ratio);
        nk_spacing(ui_menu->ctx, 1);
        if (nk_button_image_label(ui_menu->ctx, scene->play, "Start", NK_TEXT_CENTERED)) {
            fprintf(stdout, "Play!\n");
            scene->exit = 1;
        }

        nk_style_set_font(ui_menu->ctx, &scene->font_22->handle);
        nk_layout_row(ui_menu->ctx, NK_DYNAMIC, 50, 2, ratio);
        nk_spacing(ui_menu->ctx, 1);
        if (nk_button_image_label(ui_menu->ctx, scene->play, "Settings", NK_TEXT_CENTERED)) {
            fprintf(stdout, "Play!\n");
            scene->exit = 1;
        }

        nk_style_set_font(ui_menu->ctx, &scene->font_22->handle);
        nk_layout_row(ui_menu->ctx, NK_DYNAMIC, 50, 2, ratio);
        nk_spacing(ui_menu->ctx, 1);
        if (nk_button_image_label(ui_menu->ctx, scene->play, "Quit", NK_TEXT_CENTERED)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
            scene->exit = 1;
        }
    }
    nk_end(ui_menu->ctx);
    nk_glfw3_render(&ui_menu->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void render_destroy_scene(game_scene_t *scene) {
    free(scene);
}

main_menu_t *render_init_main_menu() {
    main_menu_t *main_menu = malloc(sizeof(main_menu_t));

    //mainmenu_device_init(&main_menu->device);
    asdf();
    
    {const void *image; int w, h;
    struct nk_font_config cfg = nk_font_config(0);
    cfg.oversample_h = 3; cfg.oversample_v = 2;
    nk_font_atlas_init_default(&main_menu->atlas);
    nk_font_atlas_begin(&main_menu->atlas);
    main_menu->font_14 = nk_font_atlas_add_from_file(&main_menu->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 14.0f, &cfg);
    main_menu->font_18 = nk_font_atlas_add_from_file(&main_menu->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 18.0f, &cfg);
    main_menu->font_20 = nk_font_atlas_add_from_file(&main_menu->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 20.0f, &cfg);
    main_menu->font_22 = nk_font_atlas_add_from_file(&main_menu->atlas, "resources/ui/fonts/Roboto-Regular.ttf", 22.0f, &cfg);
    image = nk_font_atlas_bake(&main_menu->atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(&main_menu->device, image, w, h);
    nk_font_atlas_end(&main_menu->atlas, nk_handle_id((int)main_menu->device.font_tex), &main_menu->device.null);}
    
    main_menu->play = image_load_icon("resources/ui/main_menu/icons/play.png");
    main_menu->settings = image_load_icon("resources/ui/main_menu/icons/play.png");
    main_menu->quit = image_load_icon("resources/ui/main_menu/icons/play.png");

    main_menu->exit = 0;


    return main_menu;
}

void render_main_menu(double delta_time, GLFWwindow* window, ui_componets_t *ui_menu, main_menu_t *main_menu) {
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    nk_glfw3_new_frame(&ui_menu->glfw);
    nk_style_set_font(ui_menu->ctx, &main_menu->font_20->handle);
    if (nk_begin(ui_menu->ctx, "Demo", nk_rect(50, 50, 250, 600),
    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
                
        //nk_layout_row_static(ui_menu->ctx, 30, 80, 1);


        static const float ratio[] = {0.15f, 0.85f};
        nk_style_set_font(ui_menu->ctx, &main_menu->font_22->handle);
        nk_layout_row(ui_menu->ctx, NK_DYNAMIC, 50, 2, ratio);
        nk_spacing(ui_menu->ctx, 1);
        if (nk_button_image_label(ui_menu->ctx, main_menu->play, "Start", NK_TEXT_CENTERED)) {
            fprintf(stdout, "Play!\n");
            main_menu->exit = 1;
        }

        nk_style_set_font(ui_menu->ctx, &main_menu->font_22->handle);
        nk_layout_row(ui_menu->ctx, NK_DYNAMIC, 50, 2, ratio);
        nk_spacing(ui_menu->ctx, 1);
        if (nk_button_image_label(ui_menu->ctx, main_menu->play, "Settings", NK_TEXT_CENTERED)) {
            fprintf(stdout, "Play!\n");
            main_menu->exit = 1;
        }

        nk_style_set_font(ui_menu->ctx, &main_menu->font_22->handle);
        nk_layout_row(ui_menu->ctx, NK_DYNAMIC, 50, 2, ratio);
        nk_spacing(ui_menu->ctx, 1);
        if (nk_button_image_label(ui_menu->ctx, main_menu->play, "Quit", NK_TEXT_CENTERED)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
            main_menu->exit = 1;
        }
    }
    nk_end(ui_menu->ctx);
    nk_glfw3_render(&ui_menu->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}


void render_destroy_main_menu(main_menu_t *main_menu) {
    free(main_menu);
}