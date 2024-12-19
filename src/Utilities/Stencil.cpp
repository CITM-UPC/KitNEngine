#include "Utilities/Stencil.h"
#include "Component/Transform.h"
#include "Structures/MeshRenderer.h"
#include <glm/gtc/type_ptr.hpp>



//void drawOutlinedObject(const std::shared_ptr<GameObject>& gameObject) {
//    if (!gameObject || !gameObject->transform) {
//        return; // Validación: Si el objeto o su transform no son válidos, salir.
//    }
//    auto meshRenderer = gameObject->GetComponentOfType<MeshRenderer>();
//    
//    glEnable(GL_STENCIL_TEST);
//    glClear(GL_STENCIL_BUFFER_BIT); 
//    glStencilFunc(GL_ALWAYS, 1, 0xFF);   
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//    glEnable(GL_DEPTH_TEST); 
//
//    
//
//    
//    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); 
//    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); 
//    glDisable(GL_DEPTH_TEST); // Deshabilitar la prueba de profundidad para el contorno
//
//    //Dibujar el contorno escalado
//    glm::mat4 worldMatrix = gameObject->transform->GetWorldMatrix();
//    glm::mat4 scaledMatrix = glm::scale(worldMatrix, glm::vec3(1.05f, 1.05f, 1.05f)); // Escala ligera
//    glUniformMatrix4fv(glGetUniformLocation(meshRenderer->shader_id, "MVP"), 1, GL_FALSE, glm::value_ptr(scaledMatrix));
//
//    glColor3f(1.0f, 0.0f, 0.0f); // Contorno rojo (puedes cambiar el color si es necesario)
//
//    // Para dibujar el contorno se usa el VAO, VBO y EBO del MeshRenderer directamente
//    
//    glBindVertexArray(meshRenderer->VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, meshRenderer->VBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRenderer->EBO);
//
//    glDrawElements(GL_TRIANGLES, meshRenderer->indices.size(), GL_UNSIGNED_INT, (void*)0); // Dibujar el contorno
//
//    // Restaurar estados de OpenGL
//    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_STENCIL_TEST);
//}
