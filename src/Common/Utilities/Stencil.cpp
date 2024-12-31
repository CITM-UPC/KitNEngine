#include "Game/Utilities/Stencil.h"
#include "Game/Component/Transform.h"
#include "Game/Component/MeshRenderer.h"
#include "Game/Component/GameObject.h"
#include "Game/Component/Camera.h"
#include <glm/gtc/type_ptr.hpp>

// Shader de contorno
GLuint outlineShaderID = 0;

void initializeOutlineShader() {
    static bool initialized = false;
    if (!initialized) {
        Shader outlineShader("Assets/Shaders/outline_vertex.glsl", "Assets/ Shaders / outline_fragment.glsl");
        outlineShaderID = outlineShader.shaderProgram;
        initialized = true;
    }
}

void drawOutlinedObject(const std::shared_ptr<GameObject>& gameObject) {
    if (gameObject == nullptr || Camera::activeCamera == nullptr) {
        return; // Validar GameObject y c�mara activa
    }

    std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponentOfType<MeshRenderer>();
    if (!meshRenderer) {
        return; // Validar MeshRenderer
    }

    initializeOutlineShader();

    // Obtener matrices de la c�mara activa
    glm::mat4 viewMatrix = Camera::activeCamera->view;
    glm::mat4 projectionMatrix = Camera::activeCamera->projection;

    // Obtener la matriz modelo del objeto
    glm::mat4 modelMatrix = gameObject->GetTransform()->GetWorldMatrix();

    // Calcular MVP para el objeto base
    glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    // Habilitar stencil buffer y configurar
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    // Dibujar el objeto base y escribir en el stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Escribir valor 1 en el buffer de stencil
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Habilitar escritura al stencil buffer
    glEnable(GL_DEPTH_TEST);

    glUseProgram(meshRenderer->shader_id);
    glUniformMatrix4fv(glGetUniformLocation(meshRenderer->shader_id, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

    glBindVertexArray(meshRenderer->VAO);
    glDrawElements(GL_TRIANGLES, meshRenderer->indices.size(), GL_UNSIGNED_INT, (void*)0);

    // Dibujar el contorno del objeto usando stencil
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Dibujar solo donde el stencil no es 1
    glStencilMask(0x00); // Deshabilitar escritura al stencil buffer
    glDisable(GL_DEPTH_TEST);

    // Calcular MVP para el contorno (escalado)
    glm::mat4 scaledModelMatrix = glm::scale(modelMatrix, glm::vec3(1.05f, 1.05f, 1.05f));
    glm::mat4 scaledMVP = projectionMatrix * viewMatrix * scaledModelMatrix;

    glUseProgram(outlineShaderID);
    glUniformMatrix4fv(glGetUniformLocation(outlineShaderID, "MVP"), 1, GL_FALSE, glm::value_ptr(scaledMVP));

    // Color del contorno
    glUniform3f(glGetUniformLocation(outlineShaderID, "outlineColor"), 1.0f, 0.0f, 1.0f); // Rosa

    glBindVertexArray(meshRenderer->VAO);
    glDrawElements(GL_TRIANGLES, meshRenderer->indices.size(), GL_UNSIGNED_INT, (void*)0);

    // Restaurar estados de OpenGL
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0xFF); // Rehabilitar escritura al stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
}