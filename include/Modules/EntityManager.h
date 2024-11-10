//
// Created by Roger on 10/11/2024.
//

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include "Module.h"
#include <vector>

#include "Component/GameObject.h"


class EntityManager : public Module {
public:
    
    EntityManager();
    ~EntityManager();

    bool Init() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool PostUpdate() override;
    bool CleanUp() override;
    
};



#endif //ENTITYMANAGER_H
