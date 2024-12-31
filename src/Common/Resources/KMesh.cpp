//
// Created by Roger on 25/12/2024.
//

#include "Game/Resources/KMesh.h"


inline void KMesh::GenerateBuffer(const GLenum type, GLuint& bufferID, const void* data, const GLsizeiptr dataSize, const GLenum usage)
{
    glGenBuffers(1, &bufferID);
    glBindBuffer(type, bufferID);
    glBufferData(type, dataSize, data, usage);
    glFinish();
}

KMesh::KMesh() : KMesh(uuids::uuid())
{
}

KMesh::KMesh(const uuids::uuid uuid) : Resource(uuid, ResourceType::Mesh)
{
}

KMesh::KMesh(const char* filepath) : Resource(uuids::uuid())
{
    
}

bool KMesh::InUse()
{
    return onGPU;
}

void KMesh::SetData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    int dataValsInVBO = GetVertexSize(flags);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    //Generate mesh vertex buffer
    GenerateBuffer(GL_ARRAY_BUFFER,VBO,vertices.data(), sizeof(GLfloat) * vertices.size(), GL_STATIC_DRAW);
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,dataValsInVBO*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    //UVs
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, dataValsInVBO*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //Generate mesh vertex_index buffer
    GenerateBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO,indices.data(), sizeof(GLuint) * indices.size(), GL_STATIC_DRAW);
    
    // //Generate mesh UV_coordinate buffer
    // if (!UVs.empty())
    // {
    //     GenerateBuffer(GL_ARRAY_BUFFER, uv_buffer_id, UVs.data(),UVs.size()*sizeof(GLfloat), GL_STATIC_DRAW);
    //     glVertexAttribPointer(2,2, GL_FLOAT, GL_TRUE, 2 * sizeof(float), (void*)0);
    // }
    

    glBindVertexArray(0);
    onGPU = true;
}

int KMesh::GetVertexSize(int flags)
{
    unsigned int ret = 3; // Sol posicio

    if (flags & MeshFlags::MeshHasUVs) ret +=2;
    if (flags & MeshFlags::MeshHasNormals) ret +=3;

    return ret;
}
