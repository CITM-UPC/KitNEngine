//
// Created by Roger on 02/10/2024.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Component.h"
#include "Transform.h"

#include <memory>
#include <vector>

// Evita instanciar GameObjects directament, utilitza CreateGameObject() en el seu lloc
class GameObject final: public Component
{
public: // Static members/functions

    static std::vector<GameObjectPtr> gameObjects;
    static GameObjectPtr selectedGameObject;

    static GameObjectPtr CreateGameObject(const GameObjectPtr& parent);
    static GameObjectPtr GetAsSmartPtr(GameObject* ptr);
    
public:
    
    GameObject(const GameObjectPtr& parentObject, const std::string& name = "GameObject");
    ~GameObject();

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool PostUpdate() override;
    bool InspectorDisplay() override;
    bool CleanUp() override;

    bool IsGameObject() const override { return true; }

    GameObject& SetParent(GameObjectPtr parent);
    
    [[nodiscard]] GameObjectPtr& GetChild(glm::uint index) { return children.at(index); }
    [[nodiscard]] std::vector<GameObjectPtr>& GetChildren() { return children; }
    [[nodiscard]] const TransformPtr GetTransform() { return transform; }

    template <typename T>
    [[nodiscard]] T& GetComponentOfType() const;

    template <typename T, typename... Args>
    requires std::derived_from<T, Component>
    std::shared_ptr<T> AddComponentOfType(Args&&... args);
    
    
    // Afegeix el component a la llista d'aquest GameObject
    ComponentPtr AddComponent(ComponentPtr& c);
    void RemoveComponent(Component* component);

    GameObject& AddChild(GameObjectPtr& g);
    GameObject& AddChild(GameObject* g);
    void RemoveChild(const GameObject* child);


public:

    GameObjectPtr parent;
    

private:

    GameObject* gameObject = this;
    
    TransformPtr transform = std::make_shared<Transform>(this);

    std::vector<ComponentPtr> components;
    std::vector<GameObjectPtr> children;

    friend Transform;
};


template <typename T, typename... Args>
requires std::derived_from<T, Component>
std::shared_ptr<T> GameObject::AddComponentOfType(Args&&... args)
{
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha derivar de GameObject");

    std::shared_ptr<T> c = std::make_shared<T>(std::forward<Args>(args)...);
    ComponentPtr componentCast = std::dynamic_pointer_cast<Component>(c);
    SetGameObject(componentCast, this);
    components.push_back(c);

    if (_awoken && c->_enabled && !c->_awoken)
    {
        c->Awake();
    }

    return c;
}

#endif //GAMEOBJECT_H
