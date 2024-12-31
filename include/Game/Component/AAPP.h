#ifndef AABB_H
#define AABB_H

#include "Game/Component/GameObject.h"
#include <glm/glm.hpp> 

// Declaraci�n de la funci�n para calcular el AABB
void CalculateAABB(const std::shared_ptr<GameObject>& gameObject);


void DrawAABB(const glm::vec3& minBounds, const glm::vec3& maxBounds);

#endif // AABB_H