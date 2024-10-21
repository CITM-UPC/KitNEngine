//
// Created by Roger on 16/10/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>

#include "SDL_opengl.h"

#define COLOR_CHANNELS 4

struct Texture {
    static GLuint nextID;
    
    std::vector<GLuint> data;
    GLuint width;
    GLuint height;
    GLuint channels = COLOR_CHANNELS;

    GLuint textureID = 0;
    
    Texture(GLuint width, GLuint height, bool autogenerate = true);
    
    Texture(std::vector<GLuint> data, GLuint width, GLuint height);

    GLuint& Get(const GLuint& x, const GLuint& y, const GLuint& channel) { return data[x + y*width + channel*width*height]; }

    void BufferTexture();
    
};



#endif //TEXTURE_H
