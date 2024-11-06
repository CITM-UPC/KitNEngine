#version 330 core
layout (location = 0) in vec3 aPos; // the position of the vertex
layout (location = 1) in vec2 aUV;

out vec2 texCoords;

uniform mat4 viewMat = mat4(1.0);

void main()
{
    gl_Position = viewMat * vec4(aPos.xyz, 1.0);
    
    texCoords = aUV;
}