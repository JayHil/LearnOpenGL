#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "Shader.h"
#include <stb_image.h>

//float factor = 1.0f;

/**
const char* vertshadersource = 
"#version 330 core \n layout (location = 0) in vec3 aPos; \n layout (location = 1) in vec3 aColor; \n out vec3 vertColor; \n void main() {\n gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n vertColor = aColor;\n }";

const char* fragshadersource = 
"#version 330 core \n out vec4 fragColor; \n in vec3 vertColor; \n void main() {\n fragColor = vec4(vertColor, 1.0);\n }";
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && factor < 1.0f) {
    //     factor = factor + 0.0001f;
    // }
    
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && factor > 0.0f) {
    //     factor = factor - 0.0001f;
    // }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Couldnt create glfw window :( " << std::endl;
        glfwTerminate();
        return -1;
    }

    //make new window the main context of the gl thread
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //initializes glad (points to header and function addresses)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not link glad to glfw" << std::endl;
        glfwTerminate();
        return -1;
    }

    //compile and link shader program from shader class.
    Shader baseShader = Shader("../vertexShader.vs", "../fragmentShader.fs");

    float verts[] = {
        // positions        //colors          //UV coords
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //top left
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //top right
        0.5f, -0.5f, 0.0f, 1.0, 1.0f, 0.0f, 1.0f, 0.0f // bottom right
    };
    
    int indicies[] = {
        0, 3, 1, 
        1, 2, 3
    };

    //VAOs get generated and set first because VAOs store VBO info.
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    //EBOs are used when you want to specify the order in which indices get drawn especially if indices are shared.
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    //SET POINTERS position, color, uvs

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // IMAGE LOAD
    
    char tex1[] = "../ashbaby.jpg";
    char tex2[] = "../container.jpg";

    unsigned int texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(tex1, &width, &height, &nrChannels, 0);

    if (data) {
        //targets Texture2d, binds the loaded texture to the target/ID from the data container.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Could not load image: " << tex1 << std::endl;
    }

    stbi_image_free(data);


    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    data = stbi_load(tex2, &width, &height, &nrChannels, 0);

    if (data) {
        //targets Texture2d, binds the loaded texture to the target/ID from the data container.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Could not load image: " << tex2 << std::endl;
    }

    stbi_image_free(data);

    //how opengl draws primitives, default is filled tris
    glPolygonMode(GL_FRONT, GL_FILL);

    baseShader.use();
    baseShader.setInt("texture1", 0);
    baseShader.setInt("texture2", 1);
    //baseShader.setFloat("factor", 0.0f);
    
    //render loop
    while(!glfwWindowShouldClose(window)) {
        //input
        processInput(window);

        //clear out old frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //THIS ACTUALLY CLEARS WINDOW
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //baseShader.use();
        //baseShader.setFloat("factor", factor);

        //render frame
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}