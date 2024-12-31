#include "Game/Component/AAPP.h"
#include "Game/Component/MeshRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <algorithm>




// Funci�n que dibuja el AABB de un GameObject utilizando OpenGL moderno
void CalculateAABB(const std::shared_ptr<GameObject>& gameObject)
{
    // Obtener MeshRenderer del GameObject
    auto meshRenderer = gameObject->GetComponentOfType<MeshRenderer>();
    if (meshRenderer == nullptr) {
        return; // No se puede calcular AABB si no hay MeshRenderer
    }

    // Obtener los v�rtices de la malla desde mainData
    const std::vector<float>& vertices = meshRenderer->mainData;

    if (vertices.empty()) {
        return; // No hay v�rtices
    }

    // Obtener la matriz de transformaci�n del modelo
    glm::mat4 transformMatrix = gameObject->GetTransform()->GetWorldMatrix();

    // Inicializar los l�mites m�nimos y m�ximos
    glm::vec3 minBounds(FLT_MAX);
    glm::vec3 maxBounds(-FLT_MAX);

    // Iterar sobre los v�rtices y calcular los l�mites
    for (size_t i = 0; i < vertices.size(); i += 5) { // mainData almacena posici�n(3), UV(2)
        glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);

        // Transformar el v�rtice con la matriz de transformaci�n
        glm::vec4 transformedVertex = transformMatrix * glm::vec4(vertex, 1.0f);

        // Actualizar los l�mites m�nimos y m�ximos
        minBounds = glm::min(minBounds, glm::vec3(transformedVertex));
        maxBounds = glm::max(maxBounds, glm::vec3(transformedVertex));
    }

    // Dibujar el AABB autom�ticamente
    DrawAABB(minBounds, maxBounds);
}

void DrawAABB(const glm::vec3& minBounds, const glm::vec3& maxBounds)
{
    // Definir las 8 esquinas del AABB
    glm::vec3 corners[] = {
        {minBounds.x, minBounds.y, minBounds.z},
        {maxBounds.x, minBounds.y, minBounds.z},
        {maxBounds.x, maxBounds.y, minBounds.z},
        {minBounds.x, maxBounds.y, minBounds.z},
        {minBounds.x, minBounds.y, maxBounds.z},
        {maxBounds.x, minBounds.y, maxBounds.z},
        {maxBounds.x, maxBounds.y, maxBounds.z},
        {minBounds.x, maxBounds.y, maxBounds.z},
    };

    // Indices para dibujar l�neas entre las esquinas
    GLuint indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0, // Base inferior
        4, 5, 5, 6, 6, 7, 7, 4, // Base superior
        0, 4, 1, 5, 2, 6, 3, 7  // Conexiones verticales
    };

    // Crear un VAO y VBO para el AABB
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configurar atributos de v�rtice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Dibujar l�neas
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    // Limpiar
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}