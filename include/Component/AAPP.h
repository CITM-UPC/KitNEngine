#ifndef AABB_H
#define AABB_H

#include "Component/GameObject.h"
#include <glm/glm.hpp> 

// Declaración de la función para calcular el AABB
void CalculateAABB(const std::shared_ptr<GameObject>& gameObject);


void DrawAABB(const glm::vec3& minBounds, const glm::vec3& maxBounds);

#endif // AABB_H