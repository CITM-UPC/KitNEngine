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

// Transformacions locals de l'objecte
class Transform final
{
public:
    Transform(GameObject* go) : gameObject(go){}
    ~Transform()
    {
        gameObject = nullptr;
    }

    [[nodiscard]] const glm::mat4& GetMatrix() const;
    [[nodiscard]] glm::vec3 GetPosition() const;
    [[nodiscard]] glm::quat GetRotation() const;
    [[nodiscard]] glm::vec3 GetScale() const;
    [[nodiscard]] glm::vec3 GetRight() const;
    [[nodiscard]] glm::vec3 GetUp() const;
    [[nodiscard]] glm::vec3 GetForward() const;

    [[nodiscard]] glm::mat4 GetWorldMatrix() const;
    [[nodiscard]] glm::vec3 GetWorldPos() const;

public:
    
    GameObject* gameObject = nullptr;

private:
    glm::mat4 M = glm::mat4(1.0f);

    glm::vec3 scale ={};
    glm::quat rotation ={};
    glm::vec3 translation ={};
};

#endif //TRANSFORM_H
