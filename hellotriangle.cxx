#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "Shader.h"
#include <stb_image.h>

float verts[] = {
    // positions        //colors          //UV coords
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //bottom left
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //top left
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //top right
    0.5f, -0.5f, 0.0f, 0.0, 0.0f, 0.0f, 1.0f, 0.0f // bottom right
};

int indicies[] = {
    0, 3, 1,
};

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not link glad to glfw" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //VAOs get generated and set first because VAOs store VBO info.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
    
    char texName[] = "container.jpg";
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texName, &width, &height, &nrChannels, 0);

    if (data) {
        //targets Texture2d, binds the loaded texture to the target/ID from the data container.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Could not load image" << texName << std::endl;
    }

    stbi_image_free(data);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //shaders with shader interface
    Shader baseShader = Shader("C:/Users/gheis/Desktop/Personal_projects/LearnOpenGL/vertexShader.vs", "C:/Users/gheis/Desktop/Personal_projects/LearnOpenGL/fragmentShader.fs");

    //how opengl draws primitives, default is filled tris
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        baseShader.use();
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram2);

    glfwTerminate();
    return 0;
}