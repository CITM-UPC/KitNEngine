//
// Created by Roger on 02/10/2024.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>

class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;

class Transform;
using TransformPtr = std::shared_ptr<Transform>;

class Transform final
{
public:
    Transform(GameObject* go) : gameObject(go){}
    ~Transform()
    {
        gameObject = nullptr;
    }

    [[nodiscard]] glm::vec3 Position() const{return glm::vec3(M[3]);}
    [[nodiscard]] glm::vec3 Rotation() const{return glm::vec3();}
    [[nodiscard]] glm::vec3 Scale() const{return glm::vec3();}
    [[nodiscard]] glm::vec3 Forward() const{return -glm::vec3(M[2]);}
    [[nodiscard]] glm::vec3 Up() const{return glm::vec3();}
    [[nodiscard]] glm::vec3 Right() const{return glm::vec3(M[0]);}
    
    
    GameObject* gameObject = nullptr;
    
    glm::mat4 M = glm::mat4(1.0f);
};

#endif //TRANSFORM_H
