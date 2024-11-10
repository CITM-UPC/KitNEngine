//
// Created by Roger on 10/11/2024.
//

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include "Input.h"
#include "Module.h"


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

    void OnDropFile(OnDropEventType& fileName);

    
};



#endif //ENTITYMANAGER_H
