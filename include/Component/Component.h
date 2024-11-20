//
// Created by Roger on 02/10/2024.
//

#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>

class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;

class Component;
using ComponentPtr = std::shared_ptr<Component>;

class Component
{
protected:
    explicit Component(GameObject* parent);
    
    Component() = default;

public:
    virtual ~Component() = default;

    virtual bool Awake() = 0;
    virtual bool Start(){ return true; }
    virtual bool PreUpdate(){ return true; }
    virtual bool Update(){ return true; }
    virtual bool PostUpdate(){ return true; }
    virtual bool InspectorDisplay(){ return true; }
    virtual bool CleanUp(){ return true; }

    virtual Component& SetGameObject(GameObject* parent);    
    virtual bool IsGameObject() const { return false; }

    bool Enable();
    bool Disable();

public:
    // El GameObject al qual està assignat aquest component
    GameObject* gameObject = nullptr;

protected:
    bool _enabled = true;
    bool _active = true;
    bool _awoken = false;

    friend GameObject;
};



#endif //COMPONENT_H
