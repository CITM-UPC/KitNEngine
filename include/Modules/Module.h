//
// Created by Roger on 02/10/2024.
//

#ifndef MODULE_H
#define MODULE_H
#include <memory>
#include <string>

class Module;
using ModulePtr = std::shared_ptr<Module>;

class Module {
public:
    
    explicit Module(const char* name, bool startEnabled = true, bool needsReinit = false) : _name(name), _active(startEnabled), _needsReinit(needsReinit)
    {
        
    }

    virtual ~Module() = default;    

    virtual bool Init() { return true; }
    virtual bool Start() { return true; }
    virtual bool PreUpdate() { return true; }
    virtual bool Update() { return true; }
    virtual bool LateUpdate() { return true; }
    virtual bool EditorUI() { return true; }
    virtual bool CleanUp() { return true; }

    bool Enable()
    {
        if (!_active)
        {
            _active = true;
        }
    }

    bool Disable()
    {
        if (_active)
        {
            _active = false;
        }
    }

    [[nodiscard]] const char* GetName() const { return _name.c_str(); }

    
protected:

    std::string _name;

    bool _active;
    bool _needsReinit;
    bool _awoken = false;
    bool _started = false;
    
};



#endif //MODULE_H
