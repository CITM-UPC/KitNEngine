//
// Created by Roger on 16/10/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>

#include "SDL_opengl.h"

#define COLOR_CHANNELS 4

struct Texture {
    std::vector<GLuint> data;
    GLuint width;
    GLuint height;
    GLuint channels = COLOR_CHANNELS;
    
    Texture(GLuint width, GLuint height, bool autogenerate = true)
    {
        // TODO acabar de generar textura checkerboard
        data.reserve(width * height * channels);
        if (autogenerate)
        for (GLuint i = 0; i < height; i++)
        {
            for (GLuint j = 0; j < width; j++)
            {
                data.push_back(0);
            }
        }
    }
    
    Texture(std::vector<GLuint> data, GLuint width, GLuint height)
    {
        this->data = data;
    }

    GLuint& operator [](const GLuint x, const GLuint y, const GLuint z)
    {
        return data[x + y*width + z*width*height];
    }
    
};



#endif //TEXTURE_H
