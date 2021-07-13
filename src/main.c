#include "main.h"


int run(void) {
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
}


int main(int argc, char *argv[]) {
    int exit_status = run();
    printf("exit(%d)\n", exit_status);
    return exit_status;
}
