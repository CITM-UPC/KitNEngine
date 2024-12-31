//
// Created by Roger on 25/12/2024.
//

#ifndef MESH_H
#define MESH_H
#include "Game/Resources/Resource.h"
#include <vector>

#include <GL/glew.h>

namespace MeshFlags
{
    enum Flags
    {
        MeshHasNoFlags = 0,
        MeshHasNormals = (1 << 0),
        MeshHasUVs = (1 << 1),
    };
}

class KMesh : public Resource {
public:
    // Retorna la quantitat de valors per vertex que s'envia a la GPU
    static int GetVertexSize(int flags);
    // Genera buffer de GPU amb els parametres especificats
    static void GenerateBuffer(GLenum type, GLuint& bufferID, const void* data, GLsizeiptr dataSize, GLenum usage);

    KMesh();
    explicit KMesh(uuids::uuid uuid);
    explicit KMesh(const char* filepath);
    bool InUse() override;

    // Envia la mesh a la GPU
    void SetData(const std::vector<float>& vector, const std::vector<unsigned int>& indices);



private:
    template <class Archive>
    void Save(Archive& archive)
    {
        archive(vertices, indices, flags);
    }
    
    template <class Archive>
    void Load(Archive& archive)
    {
        archive(vertices, indices, flags);
        SetData(vertices,indices);
    }

    bool onGPU = false;
    int flags = MeshFlags::MeshHasNoFlags;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int numVertices = 0;
    
    unsigned int VAO = 0; // Buffer ID de objecte
    unsigned int VBO = 0; // Buffer ID de vertexs
    unsigned int EBO = 0; // Buffer ID de indexs

    friend class MeshImporter;
};



#endif //MESH_H
