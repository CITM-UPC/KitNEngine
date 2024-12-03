//
// Created by Roger on 16/10/2024.
//
#include "Structures/MeshRenderer.h"

#include <iostream>

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Component/GameObject.h"
#include "glm/gtc/type_ptr.hpp"
#include "Structures/Shader.h"
#include "Structures/Texture.h"
#include "Structures/UIWindows.h"

#include "Structures/UIWindows.h"

const GLuint MeshRenderer::dataValsInVBO = 5; // position(3), UV(2)

std::vector<std::shared_ptr<MeshRenderer>> MeshRenderer::renderers = std::vector<std::shared_ptr<MeshRenderer>>();

std::vector<std::shared_ptr<MeshRenderer>> MeshRenderer::ImportMeshes(const char* filename)
{
    std::vector<std::shared_ptr<MeshRenderer>> ret;
    
    const aiScene *scene = aiImportFile(filename,aiProcess_Triangulate);

    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
        return ret;
    }

        printf("Numero de malles: %i\n", scene->mNumMeshes);
        AddLogMessage("Numero de malles: %i\n", scene->mNumMeshes);
       
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[i];
        { // Soc conscient que tal com està muntat es recorren els vèrtexs dues vegades
            printf("\nMalla %u:\n", i);
            printf(" Numero de vertexs: %u\n", mesh->mNumVertices) ;
            printf(" Numero de triangles: %u\n", mesh->mNumFaces) ;
            AddLogMessage("\nMalla %u:\n", i);
            AddLogMessage(" Numero de vertexs: %u\n", mesh->mNumVertices);
            AddLogMessage(" Numero de triangles: %u\n", mesh->mNumFaces);
            // Vèrtexs
            for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
                aiVector3D& vertex = mesh->mVertices[v] ;
                printf(" Vertex %u: (%f, %f, %f)\n", v, vertex.x, vertex.y, vertex.z) ;
                AddLogMessage(" Vertex %u: (%f, %f, %f)\n", v, vertex.x, vertex.y, vertex.z);
            }
            // Índexs de triangles (3 per triangle)
            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                aiFace& face = mesh->mFaces[f] ;
                printf(" Indexs triangle %u: ", f) ;
                AddLogMessage("[WARNING] Indexs triangle %u: ", f);
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    printf("%u ", face.mIndices[j]) ;
                    AddLogMessage("%u ", face.mIndices[j]);
                }
                printf("\n") ;
                AddLogMessage("Warning \n");
            }
        }
        std::shared_ptr<MeshRenderer> pp_mesh = std::make_shared<MeshRenderer>(mesh);
        if (!Texture::textures.empty()) pp_mesh->texture_id=Texture::textures[0].textureID;
        ret.push_back(MeshRenderer::renderers.emplace_back(pp_mesh));
		
    }
	
    aiReleaseImport(scene);
    return ret;
}

MeshRenderer::MeshRenderer(const aiMesh* mesh) : Component("MeshRenderer")
{
    mainData.reserve(mesh->mNumVertices*3);
    for (unsigned int i = 0; i < mesh->mNumVertices;i++)
    {
        mainData.push_back(mesh->mVertices[i].x);
        mainData.push_back(mesh->mVertices[i].y);
        mainData.push_back(mesh->mVertices[i].z);

        
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

MeshRenderer::~MeshRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &uv_buffer_id);
}

bool MeshRenderer::Awake()
{
    return true;
}

bool MeshRenderer::Start()
{
    return Component::Start();
}

bool MeshRenderer::PreUpdate()
{
    return Component::PreUpdate();
}

bool MeshRenderer::Update()
{
    return Component::Update();
}

void MeshRenderer::Render(const Shader* shaderProgram) const
{
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUseProgram(shaderProgram->shaderProgram);

    glm::mat4 model;
    const float* MVP;
    auto g = gameObject.lock();
    if (g != nullptr)
    {
        model = g->GetTransform()->GetBasis();
        MVP = glm::value_ptr(Camera::activeCamera->projection*Camera::activeCamera->view*model);
    }
    else
    {
        //std::cout << "Renderer not assigned to a GameObject" << std::endl;
        model = glm::mat4(1.0f);
        MVP = glm::value_ptr(model);
    }
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram->shaderProgram,"MVP"), 1, GL_FALSE, MVP);


    // no activar hasta que se tengan shaders de color funcionales
    // if (texture_id>0)
    // {
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, texture_id);
    // }


    // Finish Drawing
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    // glBindTexture(GL_TEXTURE_2D, 0);
    // glBindVertexArray(0);
    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);

    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshRenderer::Render() const
{
    Render(Shader::GetShader(shader_id).get());
}

bool MeshRenderer::PostUpdate()
{
    return Component::PostUpdate();
}

bool MeshRenderer::InspectorDisplay()
{
    return Component::InspectorDisplay();
}

bool MeshRenderer::CleanUp()
{
    return Component::CleanUp();
}

void MeshRenderer::SetUpMesh()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    //Generate mesh vertex buffer
    GenerateBuffer(GL_ARRAY_BUFFER,VBO,mainData.data(), sizeof(GLfloat) * mainData.size(), GL_STATIC_DRAW);
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
}

inline void MeshRenderer::GenerateBuffer(const GLenum type, GLuint& bufferID, const void* data, const GLsizeiptr dataSize, const GLenum usage)
{
    glGenBuffers(1, &bufferID);
    glBindBuffer(type, bufferID);
    glBufferData(type, dataSize, data, usage);
    glFinish();
}