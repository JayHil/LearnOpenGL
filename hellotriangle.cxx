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

const char* frag2source = 
"#version 330 core \n out vec4 fragColor; \n void main() {\n fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n }";

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
    unsigned int VAOs[2];
    glGenVertexArrays(2, VAOs);

    unsigned int VBOs[2];
    glGenBuffers(2, VBOs);

    /*
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
    */

    // 
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertshadersource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    
    //iv function checks to see if shader code compiles
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
    }

    //first shader in orange

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragshadersource, NULL);
    glCompileShader(fragmentShader);

    //iv function checks to see if shader code compiles
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }

    //have to make a "program" which will link the shaders we made output to input as one program for render calls.
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

    //second shader in yellow
    unsigned int frag2Shader;
    frag2Shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag2Shader, 1, &frag2source, NULL);
    glCompileShader(frag2Shader);

    glGetShaderiv(frag2Shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag2Shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, frag2Shader);
    glLinkProgram(shaderProgram2);

    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
    }

    //how opengl draws primitives, default is filled tris
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glUseProgram(shaderProgram);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);
    //glDeleteProgram(shaderProgram2);

    glfwTerminate();
    return 0;
}