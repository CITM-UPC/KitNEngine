//
// Created by Roger on 02/10/2024.
//

#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>

class GameObject;

class Component;

using ComponentPtr = std::shared_ptr<Component>;

template <typename T>
concept DerivedFromComponent = std::is_base_of_v<Component, T>;

class Component {
protected:
    Component(const std::shared_ptr<GameObject>& parent) : gameObject(parent){}
    Component() = default;

public:
    virtual ~Component() = default;

    virtual bool Awake() { return true; }
    virtual bool Start(){ return true; }
    virtual bool PreUpdate(){ return true; }
    virtual bool Update(){ return true; }
    virtual bool PostUpdate(){ return true; }
    virtual bool InspectorDisplay(){ return true; }
    virtual bool CleanUp(){ return true; }

    bool Enable();
    bool Disable();

public:
    // El GameObject al qual està assignat aquest component
    std::shared_ptr<GameObject> gameObject = nullptr;

private:
    bool _enabled = true;
};



#endif //COMPONENT_H
