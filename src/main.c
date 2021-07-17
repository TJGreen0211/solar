#include "main.h"

pthread_mutex_t loading_lock;

game_scene_t game_scene;

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


void *some_long_funciton( void *ptr )
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
    game_scene.last_state = game_scene.current_state;
    game_scene.current_state = MENU;
    pthread_mutex_unlock(&loading_lock);

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
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

    render_init_splash();
    pthread_t thread1;
    char *message1 = "Thread 1";

    if(pthread_create( &thread1, NULL, some_long_funciton, (void*) message1)) {
        return 1;
    };
    game_scene.current_state = LOADING;

    ui_componets_t * ui = init_ui(window);
    //double lag = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
		last_frame = current_frame;
        glfwPollEvents();

        if(game_scene.current_state == LOADING) {
            render_splash();
            if(game_scene.current_state == MENU) {
                pthread_join( thread1, NULL);
            }
        } else if(game_scene.current_state == MENU) {
            render_menu(delta_time, ui);
        } else {
            render_scene(delta_time);
        }

        glfwSwapBuffers(window);
        nk_clear(ui->ctx);
    }
    free(ui);
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;

}


int main(int argc, char *argv[]) {
    int exit_status = run();
    printf("exit(%d)\n", exit_status);
    return exit_status;
}
