//
// Created by Roger on 16/10/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>

#include "SDL_opengl.h"
#include "IL/il.h"

#define COLOR_CHANNELS 4

struct Texture {
    static ILuint nextID;
    
    GLint width = 0;
    GLint height = 0;

    ILuint textureID = 0;
    
    Texture(GLint width, GLint height);
    
    Texture(const std::vector<GLubyte>& data, GLint width, GLint height);

    explicit Texture(const char* path);

    void BufferTextureIL(const char* path);

    void BufferTextureGL(const GLubyte* data, GLint width, GLint height);
    
};



#endif //TEXTURE_H
