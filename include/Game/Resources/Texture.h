//
// Created by Roger on 16/10/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "Resource.h"
#include <vector>

#include <SDL2/SDL_opengl.h>
#include <IL/il.h>

#define COLOR_CHANNELS 4

class Texture : public Resource {
public:
    
    static std::vector<Texture> textures;

    static ILuint ImportTexture(const char* path);
    
    GLint width = 0;
    GLint height = 0;

    ILuint textureID = 0;
    
    Texture(GLint width, GLint height);
    
    Texture(const std::vector<GLubyte>& data, GLint width, GLint height);

    explicit Texture(const char* path);

    ~Texture();

    void BufferTextureIL(const char* path);

    void BufferTextureGL(const GLubyte* data, GLint width, GLint height);
    
};



#endif //TEXTURE_H
