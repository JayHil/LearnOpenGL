#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float verts[] = {
    -0.5f, -0.5f, 0.0f, //bottom left
    -0.25f, 0.5f, 0.0f, //left peak
    0.0f, -0.5f, 0.0f
};

float verts2[] = {
    0.0f, -0.5f, 0.0f, //middle vert
    0.25f, 0.5f, 0.0f, //right peak
    0.5f, -0.5f, 0.0f //bottom right
};

unsigned int indicies[] = {
    0, 1, 2, //first tri
    2, 3, 4 // second tri
};

const char* vertshadersource = 
"#version 330 core \n layout (location = 0) in vec3 aPos; \n void main() {\n gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n }";

const char* fragshadersource = 
"#version 330 core \n out vec4 fragColor; \n void main() {\n fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n }";

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
    //check if window init
    if (window == NULL) {
        std::cout << "Couldnt create glfw window :( " << std::endl;
        glfwTerminate();
        return -1;
    }

    //make new window the main context of the gl thread
    glfwMakeContextCurrent(window);

    //check glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not link glad to glfw" << std::endl;
        glfwTerminate();
        return -1;
    }

    //set the size of the glfw render window (the space to render inside the application window)
    glViewport(0, 0, 800, 600);

    //set up frame buffer callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //integer to store the VBO index/address on memory
    unsigned int VBO;
    //assigns memory position to VBO
    glGenBuffers(1, &VBO);
    //binds the GL array buffer object to the VBO memory position
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //loads the vertex data into the vertex array buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    //vbo 2
    unsigned int VBO2;
    //end vbo 2

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    //loads the vertex data for the triangle to the array buffer with the pragma static draw.
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertshadersource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragshadersource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glBindVertexArray(0);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glfwTerminate();
    return 0;
}