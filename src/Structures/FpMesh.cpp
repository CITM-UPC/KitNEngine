//
// Created by Roger on 29/10/2024.
//

#include "Structures/FpMesh.h"


FPMesh::FPMesh(GLfloat* data, GLuint vertexSize, GLuint vertexCount) : kMeshBase(), vertexSize(vertexSize), vertexCount(vertexCount)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GenerateBuffer(GL_ARRAY_BUFFER, VBO,data,sizeof(GLfloat)*vertexSize*vertexCount,GL_STATIC_DRAW);
}

FPMesh::~FPMesh() = default;

void FPMesh::Render(const Shader* shaderProgram) const
{
    //glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void FPMesh::GenerateBuffer(const GLenum type, GLuint& bufferID, const void* data, const GLsizeiptr dataSize, const GLenum usage)
{
    glGenBuffers(1, &bufferID);
    glBindBuffer(type, bufferID);
    glBufferData(type, dataSize, data, usage);
}
