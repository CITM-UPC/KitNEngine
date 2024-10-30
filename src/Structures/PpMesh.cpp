//
// Created by Roger on 16/10/2024.
//
#include "Structures/PpMesh.h"

#include <stdexcept>

#include "Structures/Shader.h"


GLuint PpMesh::dataValsInVBO = 8; // position(3), color(3)

PpMesh::PpMesh(const aiMesh* mesh) : kMeshBase()
{
    mainData.reserve(mesh->mNumVertices*3);
    for (unsigned int i = 0; i < mesh->mNumVertices;i++)
    {
        mainData.push_back(mesh->mVertices[i].x);
        mainData.push_back(mesh->mVertices[i].y);
        mainData.push_back(mesh->mVertices[i].z);

        // TEMPORARY FOR TESTING position data as color
        mainData.push_back(abs(mesh->mVertices[i].x));
        mainData.push_back(abs(mesh->mVertices[i].y));
        mainData.push_back(abs(mesh->mVertices[i].z));
        
        if (mesh->HasTextureCoords(0))
        {
            mainData.push_back(mesh->mTextureCoords[0][i].x);
            mainData.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            // Dummy values
            mainData.push_back(0);
            mainData.push_back(0);
        }
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

PpMesh::~PpMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &uv_buffer_id);
}

void PpMesh::Render(const Shader* shaderProgram) const
{
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindVertexArray(VAO);
    glUseProgram(shaderProgram->shaderProgram);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    // no activar hasta que se tengan shaders de color funcionales
    // if (texture_id>0)
    // {
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, texture_id);
    // }


    // Finish Drawing
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//Temporary function for shader refactor testing
void PpMesh::Render(GLuint shaderProgram) const
{
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindVertexArray(VAO);
    glUseProgram(shaderProgram);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    // no activar hasta que se tengan shaders de color funcionales
    // if (texture_id>0)
    // {
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, texture_id);
    // }


    // Finish Drawing
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void PpMesh::SetUpMesh()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    //Generate mesh vertex buffer
    GenerateBuffer(GL_ARRAY_BUFFER,VBO,mainData.data(), sizeof(GLfloat) * mainData.size(), GL_STATIC_DRAW);
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,dataValsInVBO*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    //colors
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE,dataValsInVBO*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //UVs
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, dataValsInVBO*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //Generate mesh vertex_index buffer
    GenerateBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO,indices.data(), sizeof(GLuint) * indices.size(), GL_STATIC_DRAW);
    
    // //Generate mesh UV_coordinate buffer
    // if (!UVs.empty())
    // {
    //     GenerateBuffer(GL_ARRAY_BUFFER, uv_buffer_id, UVs.data(),UVs.size()*sizeof(GLfloat), GL_STATIC_DRAW);
    //     glVertexAttribPointer(2,2, GL_FLOAT, GL_TRUE, 2 * sizeof(float), (void*)0);
    // }
    

    glBindVertexArray(0);
}

void PpMesh::GenerateBuffer(const GLenum type, GLuint& bufferID, const void* data, const GLsizeiptr dataSize, const GLenum usage)
{
    glGenBuffers(1, &bufferID);
    glBindBuffer(type, bufferID);
    glBufferData(type, dataSize, data, usage);
    glFinish();
}
