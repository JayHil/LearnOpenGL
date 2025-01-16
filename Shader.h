#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:

        unsigned int ID;

        //reads and builds shader in constructor
        Shader(const char* vertexPath, const char* fragmentPath) {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            //lets the ifstreams throw these exceptions.
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try {
                //open
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;

                //read the file buffers into the streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                //close the files
                vShaderFile.close();
                fShaderFile.close();

                //stream obj to string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch(std::ifstream::failure e) {
                std::cout << "ERROR::SHADER::CANNOT_READ_FILE" << std::endl;
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
    
            //iv function checks to see if shader code compiles
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
            }

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);

            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
            }

            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        //activates shader
        void use() {
            glUseProgram(ID);
        }

        //functions for setting uniforms
        void setBool(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (bool) value);
        }

        void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
        }

        void setFloat(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), (float) value);
        }
};

#endif
