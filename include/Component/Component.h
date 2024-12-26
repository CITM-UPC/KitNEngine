//
// Created by Roger on 02/10/2024.
//

#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>
#include <string>

class GameObject;
class Editor;


class Component
{
    //static_assert(std::derived_from<DerivedClass, Component>, "Derived class must be derived from Component");
public: // static members

    template <typename T, typename... Args>
    requires std::derived_from<T, Component>
    static std::shared_ptr<T> CreateComponentOfType(Args&&... args);

protected:
    // Do not call this directly, use a GameObject's AddComponent method instead
    explicit Component(const std::string& name = "Unnamed");
    

public:
    virtual ~Component() = default;

    virtual bool Awake() = 0;
    virtual bool Start();
    virtual bool PreUpdate();
    virtual bool Update();
    virtual bool PostUpdate();
    virtual bool InspectorDisplay();
    virtual bool CleanUp();

    template <class Archive>
    bool Save(Archive& ar);

    static std::shared_ptr<Component> SetGameObject(std::shared_ptr<GameObject>& newParent,
                                                    std::shared_ptr<Component>& component);    
    [[nodiscard]] virtual bool IsGameObject() const;

    bool Enable();
    bool Disable();

    [[nodiscard]] bool IsActive() const;

    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] std::shared_ptr<GameObject> GetGameObject(bool ExceptOnNull = true) const;
    [[nodiscard]] std::shared_ptr<Component> GetSmartPtr() const;

protected:
    // El GameObject al qual està assignat aquest component. En gameobjects es una referència a si mateix
    std::weak_ptr<GameObject> gameObject;

    std::string _name;

    std::weak_ptr<Component> _selfPtr;

protected:
    // Active next frame (change to enable/disable next frame)
    bool _enabled = true;
    // Active this frame (don't directly change this)
    bool _active = true;
    // Component's initial configuration set
    bool _awoken = false;

    friend class GameObject;
    friend class Editor;
};

template <typename T, typename ... Args>
requires std::derived_from<T, Component>
std::shared_ptr<T> Component::CreateComponentOfType(Args&&... args)
{
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha derivar de GameObject. Utilitza GameObject::CreateGameObject");

    auto ret = std::make_shared<T>(std::forward<Args>(args)...);
    ret->_selfPtr = ret;
    
    return ret;
}


#endif //COMPONENT_H
