#version 330 core

in vec3 aPos;
out vec3 vertexColor;

void main () {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = vec3(1.0, 1.0, 0.0);
}