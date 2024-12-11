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
    virtual bool PreUpdate() { _active = _enabled; return _active; }
    virtual bool Update() { return _active; }
    virtual bool LateUpdate() { return _active; }
    virtual bool EditorUI() { return _active; }
    virtual bool CleanUp() = 0;

    bool Enable()
    {
        if (!_enabled)
        {
            _enabled = true;
        }
        return _enabled;
    }

    bool Disable()
    {
        if (_enabled)
        {
            _enabled = false;
        }
        return _enabled;
    }

    [[nodiscard]] const char* GetName() const { return _name.c_str(); }

    
protected:

    std::string _name;

    // Active next frame (change to enable/disable next frame)
    bool _enabled = true;
    // Active this frame (don't directly change this)
    bool _active = true;
    
    bool _needsReinit;
    bool _awoken = false;
    bool _started = false;
    
};



#endif //MODULE_H
