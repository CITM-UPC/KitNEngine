//
// Created by Roger on 13/11/2024.
//
#include "Component/Transform.h"

#include "Component/GameObject.h"
#include "glm/gtx/quaternion.hpp"

#include <imgui.h>

#include "Structures/UIWindows.h"

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
    return {
        glm::length(basis[0]),
        glm::length(basis[1]),
        glm::length(basis[2])
        };
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
    if (gameObject->parent == nullptr)
        return position;
    else
        return gameObject->parent->transform->GetWorldPos()+position;
}

void Transform::SetPosition(const glm::vec3& position)
{
    this->position = position;
}

void Transform::SetRotation(const glm::quat& rotation)
{
    basis = glm::toMat3(rotation);
}

void Transform::SetScale(const glm::vec3& scale)
{
    basis[0] = glm::normalize(basis[0]) * scale.x;
    basis[1] = glm::normalize(basis[1]) * scale.y;
    basis[2] = glm::normalize(basis[2]) * scale.z;
}

void Transform::LookAt(const glm::vec3& target, bool worldUp)
{
    basis = glm::lookAt(GetPosition(),target,worldUp ? glm::vec3(0,1,0) : GetUp() );
}

void Transform::InspectorDisplay(ImGuiInputTextFlags inputFlags)
{
    float pos[3] = { position.x, position.y, position.z };
    glm::vec3 rotVec = glm::degrees(glm::eulerAngles(glm::quat_cast(basis)));
    float rot[3] = { rotVec.x, rotVec.y, rotVec.z };
    if (ImGui::InputFloat3("Position:##TransformPos", pos, "%.1f", inputFlags))
    {
        position.x = pos[0];
        position.y = pos[1];
        position.z = pos[2];
        std::string str = "Position: "+std::to_string(position.x)+std::to_string(position.y)+std::to_string(position.z);
        AddLogMessage(str.c_str());
    }
    if (ImGui::InputFloat3("Rotation:##TransformRot", rot, "%.1f", inputFlags))
    {
        rotVec.x = rot[0];
        rotVec.y = rot[1];
        rotVec.z = rot[2];
        SetRotation(glm::quat(glm::radians(rotVec)));
        std::string str = "Rotation: (x:%f, y:%f, z:%f)";
        AddLogMessage(str.c_str(), rot[0], rot[1], rot[2]);
    }
}
