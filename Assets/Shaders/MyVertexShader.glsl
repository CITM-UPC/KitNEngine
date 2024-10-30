#version 330 core
layout (location = 0) in vec3 aPos; // the position of the vertex
layout (location = 1) in vec3 aColor; // the color of the vertex  
layout (location = 2) in vec2 aUV;

out vec3 vertexColor; // specify a color output to the fragment shader
out vec2 texCoords;

void main()
{
    gl_Position = vec4(aPos.xyz, 1.0);
    
    vertexColor = aColor;
    texCoords = aUV;
}