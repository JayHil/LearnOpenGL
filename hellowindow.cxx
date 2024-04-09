#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


/**
 * On window resizes, changes the viewport to match the new window dimensions
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

/**
 * processes keyboard inputs for the passed window.
*/
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //checks to see if glfw is correctly linked
    GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Unable to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //makes the window we made the main context on this thread
    glfwMakeContextCurrent(window);

    //checks to see if glad is successfully linked
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Unable to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    //updates the viewport each time the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //renderloop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        //rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
