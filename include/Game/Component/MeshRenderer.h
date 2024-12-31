//
// Created by Roger on 09/10/2024.
//

#ifndef MESHRENDERER_H
#define MESHRENDERER_H
#include "Component.h"

#include <GL/glew.h>
#include <assimp/mesh.h>
#include <memory>
#include <vector>


class Shader;

class MeshRenderer : public Component
{
public:
    static const GLuint dataValsInVBO;
    static std::vector<std::shared_ptr<MeshRenderer>> renderers;

    static std::vector<std::shared_ptr<MeshRenderer>> ImportMeshes(const char* filename, bool createGameObjects = false);
    
    explicit MeshRenderer(const aiMesh* mesh);
    ~MeshRenderer();
    
    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    void Render(const Shader* shaderProgram) const;
    void Render() const;
    bool PostUpdate() override;
    bool InspectorDisplay() override;
    bool CleanUp() override;

    std::vector<GLfloat> mainData;
    std::vector<GLuint> indices;
    std::vector<GLfloat> UVs;
    
    GLuint VAO = 0; // Buffer de modelo entero
    GLuint VBO = 0; // Buffer de vertices
    GLuint EBO = 0; // Buffer de indices
    GLuint uv_buffer_id = 0; // Buffer de UVs
    
    GLuint shader_id = 0; // ID de shader en la lista de shaders (NO EN GPU)
    GLuint texture_id = 0; // ID de textura a aplicar

private:

    void SetUpMesh();
};

#endif //MESHRENDERER_H
