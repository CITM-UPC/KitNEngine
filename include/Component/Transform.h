//
// Created by Roger on 02/10/2024.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>

class GameObject;

using GameObjectPtr = std::shared_ptr<GameObject>;

class Transform : public Component
{
public:
    Transform(GameObject* go) : gameObject(go){}
    ~Transform() = default;

    [[nodiscard]] glm::vec3 Position() const{return glm::vec3(_M[3]);}
    [[nodiscard]] glm::vec3 Rotation() const{return glm::vec3();}
    [[nodiscard]] glm::vec3 Scale() const{return glm::vec3();}
    [[nodiscard]] glm::vec3 Forward() const{return -glm::vec3(_M[2]);}
    [[nodiscard]] glm::vec3 Up() const{return glm::vec3();}
    [[nodiscard]] glm::vec3 Right() const{return glm::vec3(_M[0]);}
    
    
    GameObjectPtr gameObject;
    
    glm::mat4 _M;
};

#endif //TRANSFORM_H
