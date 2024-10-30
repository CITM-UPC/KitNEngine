#version 330 core
out vec4 FragColor;
  
in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)  
in vec2 texCoords;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
    //FragColor = vec4(texCoords.xy, (texCoords.x+texCoords.y)/2,1.0);
}