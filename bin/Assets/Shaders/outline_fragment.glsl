#version 330 core

out vec4 FragColor;

uniform vec3 outlineColor; // Color del contorno

void main()
{
    FragColor = vec4(outlineColor, 0.70); // Color sólido con alpha = 1.0
}