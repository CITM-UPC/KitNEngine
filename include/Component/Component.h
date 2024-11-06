//
// Created by Roger on 02/10/2024.
//

#ifndef COMPONENT_H
#define COMPONENT_H

class Component;

using ComponentPtr = std::shared_ptr<Component>;

template <typename T>
concept DerivedFromComponent = std::is_base_of_v<Component, T>;

class Component {
protected:
    Component() = default;
    virtual ~Component() = default;

public:

    virtual void Awake(){}
    virtual void Start(){}
    virtual void PreUpdate(){}
    virtual void Update(){}
    virtual void PostUpdate(){}
    virtual void InspectorDisplay(){}
    
};



#endif //COMPONENT_H
