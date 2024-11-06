//
// Created by Roger on 29/10/2024.
//

#ifndef KMESHBASE_H
#define KMESHBASE_H
#include "GL/glew.h"
#include <memory>
#include <vector>


struct PpMesh;
class Shader;
class kMeshBase {
public:


    GLuint VAO = 0; // Buffer de modelo entero
    GLuint VBO = 0; // Buffer de vertices
    GLuint EBO = 0; // Buffer de indices
    GLuint uv_buffer_id = 0; // Buffer de UVs
    
public:
    kMeshBase() = default;
    virtual ~kMeshBase() = default;

    virtual void Render(const Shader* shaderProgram) const = 0;
    virtual void Render(GLuint shaderProgram) const = 0;
    
};



#endif //KMESHBASE_H
