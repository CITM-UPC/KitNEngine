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
    // Do not call this directly, use a GameObject's AddComponent method instead
    explicit Component() = default;
    

public:
    virtual ~Component() = default;

    virtual bool Awake() = 0;
    virtual bool Start();
    virtual bool PreUpdate();
    virtual bool Update();
    virtual bool PostUpdate();
    virtual bool InspectorDisplay();
    virtual bool CleanUp();

    static ComponentPtr SetGameObject(ComponentPtr& component, GameObject* newParent);    
    [[nodiscard]] virtual bool IsGameObject() const;

    bool Enable();
    bool Disable();

    [[nodiscard]] bool IsActive() const;

public:
    // El GameObject al qual està assignat aquest component
    GameObject* gameObject = nullptr;

protected:
    // Active next frame (change to enable/disable next frame)
    bool _enabled = true;
    // Active this frame (don't directly change this)
    bool _active = true;
    // Component's initial configuration set
    bool _awoken = false;

    friend GameObject;
};



#endif //COMPONENT_H
