//
// Created by Roger on 16/10/2024.
//
#include "Structures/MyMesh.h"


glm::uint MyMesh::nextBufferId = 1;

MyMesh::MyMesh(const aiMesh* mesh)
{
    vertices.reserve(mesh->mNumVertices*3);
    for (unsigned int i = 0; i < mesh->mNumVertices;i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
    }
        
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    SetUpMesh();
}

void MyMesh::StartDraw()
{
    glBindVertexArray(vertex_array_id);
}

void MyMesh::EndDraw()
{
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MyMesh::SetUpMesh()
{
    vertex_array_id = nextBufferId++;
    vertex_buffer_id = nextBufferId++;
    index_buffer_id = nextBufferId++;

    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);
    
    //Generate mesh vertex buffer
    GenerateBuffer(GL_ARRAY_BUFFER,vertex_buffer_id,vertices.data(),vertices.size()*sizeof(float), GL_STATIC_DRAW);
    // glGenBuffers(1, (GLuint*) &(vertex_buffer_id));
    // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    // glBufferData (GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    //Generate mesh vertex_index buffer
    GenerateBuffer(GL_ELEMENT_ARRAY_BUFFER,index_buffer_id,indices.data(),indices.size()*sizeof(unsigned int), GL_STATIC_DRAW);
    // glGenBuffers(1, (GLuint*) &(index_buffer_id));
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 3 floats per vertex (x, y, z)
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void MyMesh::GenerateBuffer(const GLenum type, GLuint& bufferID, const void* data, const GLsizeiptr dataSize, const GLenum usage)
{
    glGenBuffers(1, (GLuint*)&bufferID);
    glBindBuffer(type, bufferID);
    glBufferData(type, dataSize, data, usage);
}
