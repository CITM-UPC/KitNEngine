//
// Created by Roger on 25/12/2024.
//

#include "Game/Importers/MeshImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Editor/Misc/UIWindows.h"

void MeshImporter::Import(const char* filename)
{
    Assimp::Importer importer;

    unsigned int importFlags = 0;
    importFlags |= aiProcess_Triangulate;
    
    const aiScene *scene = importer.ReadFile(filename,importFlags);


    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", importer.GetErrorString());
        AddLogMessage("Error en carregar el fitxer: %s\n", importer.GetErrorString());
        return;
    }
    
    if (scene->HasMeshes())
        AddLogMessage("Numero de malles: %i\n", scene->mNumMeshes);

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        AddLogMessage("\nMalla %u:\n", i);
        ImportMesh(mesh);
		
    }
	
    importer.FreeScene();
}

void MeshImporter::ImportMesh(const aiMesh* mesh)
{
    KMesh newMesh;
    
    
    AddLogMessage(" Numero de vertexs: %u\n", mesh->mNumVertices);
    AddLogMessage(" Numero de triangles: %u\n", mesh->mNumFaces);
    
    newMesh.vertices.reserve(mesh->mNumVertices*3);
    for (unsigned int i = 0; i < mesh->mNumVertices;i++)
    {
        newMesh.vertices.push_back(mesh->mVertices[i].x);
        newMesh.vertices.push_back(mesh->mVertices[i].y);
        newMesh.vertices.push_back(mesh->mVertices[i].z);
        AddLogMessage("Vertex %u: (x:%f, y:%f, z:%f", i, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        
        if (mesh->HasTextureCoords(0))
        {
            newMesh.vertices.push_back(mesh->mTextureCoords[0][i].x);
            newMesh.vertices.push_back(mesh->mTextureCoords[0][i].y);
            AddLogMessage(", u:%f, v:%f", mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            // Dummy values
            newMesh.vertices.push_back(0);
            newMesh.vertices.push_back(0);
            AddLogMessage(", UV:No");
        }
        
        AddLogMessage(")\n");
    }
        
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        AddLogMessage("Face %u", i);
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            newMesh.indices.push_back(face.mIndices[j]);
            AddLogMessage(" %u", face.mIndices[j]);
        }
    }
    
}
