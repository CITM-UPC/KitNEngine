#version 330 core

layout (location = 0) in vec3 aPos; // Posición del vértice
uniform mat4 MVP; // Matriz de modelo-vista-proyección

void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
}