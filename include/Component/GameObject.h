//
// Created by Roger on 02/10/2024.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <vector>

#include "Component.h"
#include "Transform.h"

class GameObject : public Component
{
    public:
    GameObject();
    ~GameObject();

    
    
private:
    
    Transform* transform;

    std::vector<Component*> components;
    std::vector<GameObject*> children;
};

#endif //GAMEOBJECT_H
