//
// Created by Roger on 29/10/2024.
//

#ifndef FPMESH_H
#define FPMESH_H
#include "Structures/KMeshBase.h"


// Mesh para pipeline fija
struct FPMesh : kMeshBase{
public:
    GLuint VAO = 0, VBO = 0, EBO = 0;

    GLuint vertexSize;
    GLuint vertexCount;

public:
    
    FPMesh(GLfloat* data, GLuint vertexSize, GLuint vertexCount);
    ~FPMesh() override;
    void Render(const Shader* shaderProgram) const override;
    void Render(GLuint shaderProgram) const override {}
    
private:
    
    void GenerateBuffer(GLenum type, GLuint& bufferID, const void* data, GLsizeiptr dataSize, GLenum usage);
};



#endif //FPMESH_H
