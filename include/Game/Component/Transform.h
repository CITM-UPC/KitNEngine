//
// Created by Roger on 02/10/2024.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class GameObject;

class Transform;
using TransformPtr = std::shared_ptr<Transform>;
typedef int ImGuiInputTextFlags;

// Transformacions locals de l'objecte
// TODO Test de totes les funcions de transform
class Transform final
{
public:
    Transform(GameObject* go) : gameObject(go){}
    ~Transform()
    {
        gameObject = nullptr;
    }

    [[nodiscard]] glm::mat4 GetBasis() const;
    [[nodiscard]] glm::vec3 GetPosition() const;
    [[nodiscard]] glm::quat GetRotation() const;
    [[nodiscard]] glm::vec3 GetScale() const;
    [[nodiscard]] glm::vec3 GetRight() const;
    [[nodiscard]] glm::vec3 GetUp() const;
    [[nodiscard]] glm::vec3 GetForward() const;

    [[nodiscard]] glm::mat4 GetWorldMatrix() const;
    [[nodiscard]] glm::mat3 GetWorldBasis() const;
    [[nodiscard]] glm::vec3 GetWorldPos() const;

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetScale(glm::vec3& scale);
    void LookAt(const glm::vec3& target, bool worldUp = false);

    void InspectorDisplay(ImGuiInputTextFlags& inputFlags);

public:
    
    GameObject* gameObject = nullptr;

private:
    glm::mat3 basis = glm::mat3(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
};

#endif //TRANSFORM_H
