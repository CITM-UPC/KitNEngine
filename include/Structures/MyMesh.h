//
// Created by Roger on 09/10/2024.
//

#ifndef MODEL3D_H
#define MODEL3D_H

#include "GL/glew.h"
#include <vector>

#include "assimp/mesh.h"
#include "glm/fwd.hpp"

struct MyMesh
{
public:

    static glm::uint nextBufferId;

    explicit MyMesh(const aiMesh* mesh);

    void StartDraw();
    
    void EndDraw();

    std::vector<float> vertices;
    std::vector<glm::uint> indices;
    GLuint vertex_array_id = 0;
    GLuint vertex_buffer_id = 0;
    GLuint index_buffer_id = 0;

private:

    void SetUpMesh();

    static void GenerateBuffer(GLenum type, GLuint& bufferID, const void* data, GLsizeiptr dataSize, GLenum usage);
    
};

#endif //MODEL3D_H
