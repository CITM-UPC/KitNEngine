#version 330 core
layout (location = 0) in vec3 aPos; // the position of the vertex
layout (location = 1) in vec2 aUV;

out vec2 texCoords;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 model = mat4(1.0);

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
    //gl_Position = model * vec4(aPos.xyz, 1.0);
    
    texCoords = vec2(-aUV.x, aUV.y);
}