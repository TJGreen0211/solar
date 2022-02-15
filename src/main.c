#include "main.h"

pthread_mutex_t loading_lock;

static arcball_camera camera;
static int actionPress, keys;

game_state_t game_state;

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


void *some_long_function( void *ptr )
{
    float asdf = 0.0;
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);
    for(int i = 0; i < 100000000; i++) {
        asdf += 1.5;
    }
    printf("End function: %f\n", asdf);
    pthread_mutex_lock(&loading_lock);
    game_state.last_state = game_state.current_state;
    game_state.current_state = MENU;
    game_state.changed_state = 1;
    pthread_mutex_unlock(&loading_lock);

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void process_movement(arcball_camera *camera, double delta_time) {
	if(keys == GLFW_KEY_W && actionPress == GLFW_PRESS)
        processKeyboard(camera, FORWARD, delta_time);
    if(keys == GLFW_KEY_S && actionPress == GLFW_PRESS)
        processKeyboard(camera, BACKWARD, delta_time);
    if(keys == GLFW_KEY_A && actionPress == GLFW_PRESS)
        processKeyboard(camera, LEFT, delta_time);
    if(keys == GLFW_KEY_D && actionPress == GLFW_PRESS)
        processKeyboard(camera, RIGHT, delta_time);
    if(keys == GLFW_KEY_SPACE && actionPress == GLFW_PRESS)
        processKeyboard(camera, UP, delta_time);
    if(keys == GLFW_KEY_LEFT_SHIFT && actionPress == GLFW_PRESS)
        processKeyboard(camera, DOWN, delta_time);
    if(keys == GLFW_KEY_Q && actionPress == GLFW_PRESS)
        processKeyboard(camera, LEFT_ROLL, delta_time);
    if(keys == GLFW_KEY_E && actionPress == GLFW_PRESS)
        processKeyboard(camera, RIGHT_ROLL, delta_time);
}


int run(void) {
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(mode->width, mode->height, "Solar", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    int window_width = 0, window_height = 0;
    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &window_width, &window_height);
    glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    double delta_time = 0.0;
    double last_frame = 0.0;

    
    pthread_t thread1;
    char *message1 = "Thread 1";

    if(pthread_create( &thread1, NULL, some_long_function, (void*) message1)) {
        return 1;
    };
    game_state.current_state = LOADING;
    game_state.changed_state = 0;

    splash_screen_t *splash_screen = render_init_splash();
    ui_componets_t *ui = init_ui(window);
    main_menu_t *main_menu;
    game_scene_t *scene;
    arcball_camera_init(&camera, 90.0, (float)mode->width/(float)mode->height, 0.1, 5000.0);
    
    //double lag = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
		last_frame = current_frame;
        

        if(game_state.current_state == LOADING || game_state.changed_state) {
            render_splash(splash_screen);
            if(game_state.last_state == LOADING) {
                pthread_join( thread1, NULL);
                render_destroy_splash(splash_screen);
                printf("MAIN MENU INIT\n");
                main_menu = render_init_main_menu();
                game_state.changed_state = 0;
            }
        } else if(game_state.current_state == MENU) {
            render_main_menu(delta_time, window, ui, main_menu);
            if(main_menu->exit) {
                render_destroy_main_menu(main_menu);
                game_state.current_state = RUNNING;
                scene = render_init_scene();
            }
        } else {
            process_movement(&camera, delta_time);
            render_scene(delta_time, window, ui, scene);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
        nk_clear(ui->ctx);
    }
    free(ui);
    free(scene);
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;

}


int main(int argc, char *argv[]) {
    int exit_status = run();
    printf("exit(%d)\n", exit_status);
    return exit_status;
}
