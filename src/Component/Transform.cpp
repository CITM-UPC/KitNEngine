//
// Created by Roger on 13/11/2024.
//
#include "Component/Transform.h"

#include "Component/GameObject.h"

const glm::mat4& Transform::GetMatrix() const
{
    return M;
}

glm::vec3 Transform::GetPosition() const
{
    return M[3];
}

glm::quat Transform::GetRotation() const
{
    return glm::quat_cast(M);
}

glm::vec3 Transform::GetScale() const
{
    return glm::vec3(
        glm::length(M[0]),
        glm::length(M[1]),
        glm::length(M[2])
        );
}

glm::vec3 Transform::GetRight() const
{
    return glm::normalize(glm::vec3(M[0][0], M[1][0], M[2][0]));
}

glm::vec3 Transform::GetUp() const
{
    return glm::normalize(glm::vec3(M[0][1], M[1][1], M[2][1]));
}

glm::vec3 Transform::GetForward() const
{
    return glm::normalize(glm::vec3(M[0][2], M[1][2], M[2][2]));
}

glm::mat4 Transform::GetWorldMatrix() const
{
    if (gameObject->parent == nullptr)
        return M;
    return gameObject->parent->GetTransform()->GetWorldMatrix() * this->M;
}

glm::vec3 Transform::GetWorldPos() const
{
    return GetWorldMatrix()*M[3];
}
