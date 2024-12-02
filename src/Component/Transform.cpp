//
// Created by Roger on 13/11/2024.
//
#include "Component/Transform.h"

#include <stdexcept>

#include "Component/GameObject.h"
#include "glm/gtx/quaternion.hpp"

glm::mat4 Transform::GetBasis() const
{
    return basis;
}

glm::vec3 Transform::GetPosition() const
{
    return position;
}

glm::quat Transform::GetRotation() const
{
    return glm::quat_cast(basis);
}

glm::vec3 Transform::GetScale() const
{
    return glm::vec3(
        glm::length(basis[0]),
        glm::length(basis[1]),
        glm::length(basis[2])
        );
}

glm::vec3 Transform::GetRight() const
{
    return glm::normalize(glm::vec3(basis[0][0], basis[1][0], basis[2][0]));
}

glm::vec3 Transform::GetUp() const
{
    return glm::normalize(glm::vec3(basis[0][1], basis[1][1], basis[2][1]));
}

glm::vec3 Transform::GetForward() const
{
    return glm::normalize(glm::vec3(basis[0][2], basis[1][2], basis[2][2]));
}

glm::mat4 Transform::GetWorldMatrix() const
{
    
    return glm::translate(glm::mat4(GetWorldBasis()), GetWorldPos());
}

glm::mat3 Transform::GetWorldBasis() const
{
    if (gameObject->parent == nullptr)
        return basis;
    else
        return gameObject->parent->transform->GetWorldBasis()*basis;
}

glm::vec3 Transform::GetWorldPos() const
{
    return (GetWorldMatrix()*glm::vec4(position,1.0f));
}

void Transform::SetPosition(const glm::vec3& position)
{
    basis[3] = glm::vec4(position,basis[3][3]);
}

void Transform::SetRotation(const glm::quat& rotation)
{
    basis = glm::toMat4(rotation);
}

void Transform::SetScale(const glm::vec3& scale)
{
    basis = glm::scale(glm::mat4(),scale);
}

void Transform::LookAt(const glm::vec3& target, bool worldUp)
{
    basis = glm::lookAt(GetPosition(),target,worldUp ? glm::vec3(0,1,0) : GetUp() );
}
