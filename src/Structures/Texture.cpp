//
// Created by Roger on 16/10/2024.
//

#include "../../include/Structures/Texture.h"

#include <stdexcept>

GLuint Texture::nextID = 1;

Texture::Texture(GLuint width, GLuint height, bool autogenerate)
{
    // TODO acabar de generar textura checkerboard
    data.reserve(width * height * channels);
    textureID = nextID++;
    if (autogenerate)
        for (GLuint i = 0; i < height; i++)
        {
            for (GLuint j = 0; j < width; j++)
            {
                int c = ((((i&0x8)==0)^(((j&0x8))==0)))*255;
                Get(j,i,0) = c;
                Get(j,i,1) = c;
                Get(j,i,2) = c;
                Get(j,i,3) = 255;
            }
        }
}

Texture::Texture(std::vector<GLuint> data, GLuint width, GLuint height)
{
    this->data = data;
    this->width = width;
    this->height = height;

    this->textureID = nextID++;

    BufferTexture();
}

void Texture::BufferTexture()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // U
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // V
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, data.data());
}
