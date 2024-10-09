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


    glm::vec3 position;
    glm::vec4 rotation;
    glm::vec3 scale;
};

#endif //TRANSFORM_H
