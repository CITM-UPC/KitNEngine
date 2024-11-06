//
// Created by Roger on 02/10/2024.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Transform : public Component
{
public:
    Transform();
    ~Transform();


    [[nodiscard]] glm::vec3 Position() const;
    [[nodiscard]] glm::vec3 Rotation() const;
    [[nodiscard]] glm::vec3 Scale() const;
    [[nodiscard]] glm::vec3 Forward() const;
    [[nodiscard]] glm::vec3 Up() const;
    [[nodiscard]] glm::vec3 Right() const;
    
    
    glm::vec3 position;
    glm::vec4 rotation;
    glm::vec3 scale;
};

#endif //TRANSFORM_H
