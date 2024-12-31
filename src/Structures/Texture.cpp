//
// Created by Roger on 16/10/2024.
//

#include "../../include/Structures/Texture.h"

#include <iostream>
#include <ostream>

std::vector<Texture> Texture::textures;

ILuint Texture::ImportTexture(const char* path)
{
    std::cout << "Importing texture: " << path << std::endl;
    return textures.emplace_back(path).textureID;
}

// TODO Assignar correctament UUID
Texture::Texture(const GLint width, const GLint height) : Resource(uuids::uuid(), ResourceType::Texture)
{
    std::vector<GLubyte> data(width*height*COLOR_CHANNELS);
    GLint size = width*height;
    for (GLint i = 0; i < height; i++)
    {
        for (GLint j = 0; j < width; j++)
        {
            int c = ((((i&0x8)==0)^(((j&0x8))==0)))*255;
            data[j+i*width+size*0] = c;
            data[j+i*width+size*1] = c;
            data[j+i*width+size*2] = c;
            data[j+i*width+size*3] = 255;
        }
    }
    this->width = width;
    this->height = height;

    BufferTextureGL(data.data(), width, height);
}

// TODO Assignar correctament UUID
Texture::Texture(const std::vector<GLubyte>& data, const GLint width, const GLint height) : Resource(uuids::uuid(), ResourceType::Texture)
{
    this->width = width;
    this->height = height;

    BufferTextureGL(data.data(), width, height);
}

// TODO Assignar correctament UUID
Texture::Texture(const char* path) : Resource(uuids::uuid(), ResourceType::Texture)
{
    BufferTextureIL(path);
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureID);
}

void Texture::BufferTextureIL(const char* path)
{
    // DevIL texture
    ilGenImages(1, &this->textureID);
    ilBindImage(this->textureID);
    ILboolean success = ilLoadImage(path);
    if (!success)
    {
        std::cerr << "Failed to load image: " << path << std::endl;
        return;
    }
    
    success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    if (!success)
    {
        std::cerr << "Failed to convert image to RGBA: " << path << std::endl;
    }

    width = ilGetInteger(IL_IMAGE_WIDTH);
    height = ilGetInteger(IL_IMAGE_HEIGHT);

    BufferTextureGL(ilGetData(), width, height);

    ilDeleteImages(1, &this->textureID);
    
}

void Texture::BufferTextureGL(const GLubyte* data, const GLint width, const GLint height)
{
    // OpenGL texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // U
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // V
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
}
