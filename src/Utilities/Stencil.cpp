#include "Utilities/Stencil.h"
#include "Component/Transform.h"
#include "Structures/MeshRenderer.h"
#include "Component/GameObject.h"
#include "Component/Camera.h"
#include <glm/gtc/type_ptr.hpp>

GLuint outlineShaderID = 1;

void drawOutlinedObject(const std::shared_ptr<GameObject>& gameObject) {
    if (gameObject == nullptr || Camera::activeCamera == nullptr) {
        return; // Validar GameObject y cámara activa
    }

    std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponentOfType<MeshRenderer>();
    if (!meshRenderer) {
        return; // Validar MeshRenderer
    }

    // Obtener matrices de la cámara activa
    glm::mat4 viewMatrix = Camera::activeCamera->view;
    glm::mat4 projectionMatrix = Camera::activeCamera->projection;

    // Obtener la matriz modelo del objeto
    glm::mat4 modelMatrix = gameObject->GetTransform()->GetWorldMatrix();

    // Calcular MVP para el objeto base
    glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    // Habilitar stencil buffer y configurar
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    // Dibujar el objeto base (escribir en el stencil buffer)
    //glStencilFunc(GL_ALWAYS, 1, 0xFF); // Escribir valor 1 en el buffer de stencil
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glEnable(GL_DEPTH_TEST);

    //// Usar el shader del MeshRenderer y enviar el MVP
    //glUseProgram(meshRenderer->shader_id);
    //glUniformMatrix4fv(glGetUniformLocation(meshRenderer->shader_id, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

    //glBindVertexArray(meshRenderer->VAO);
    //glDrawElements(GL_TRIANGLES, meshRenderer->indices.size(), GL_UNSIGNED_INT, (void*)0);

    // Dibujar el contorno del objeto (usar stencil)
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Dibujar solo donde el stencil no es 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDisable(GL_DEPTH_TEST); // Deshabilitar prueba de profundidad para evitar solapar el objeto base

    // Calcular MVP para el contorno (con escala ligera)
    glm::mat4 scaledModelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
    glm::mat4 scaledMVP = projectionMatrix * viewMatrix * scaledModelMatrix;

    // Usar el shader para el contorno (todo de un color sólido)
    glUseProgram(outlineShaderID);
    glUniformMatrix4fv(glGetUniformLocation(outlineShaderID, "MVP"), 1, GL_FALSE, glm::value_ptr(scaledMVP));

    // Definir color del contorno (rosa)
    glUniform3f(glGetUniformLocation(outlineShaderID, "outlineColor"), 0.0f, 0.0f, 0.0f); // Contorno rosa

    glBindVertexArray(meshRenderer->VAO);
    glDrawElements(GL_TRIANGLES, meshRenderer->indices.size(), GL_UNSIGNED_INT, (void*)0);

    // Restaurar estados de OpenGL
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}
