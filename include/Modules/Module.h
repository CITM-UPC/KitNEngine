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
    
    explicit Module(const char* name) : _name(name)
    {
        
    }

    virtual ~Module() = default;    

    virtual bool Init() { return true; }

    virtual bool Start() { return true; }

    virtual bool PreUpdate() { return true; }
    
    virtual bool Update() { return true; }

    virtual bool PostUpdate() { return true; }

    virtual bool CleanUp() { return true; }

    [[nodiscard]] const char* GetName() const { return _name.c_str(); }

    
private:

    std::string _name;
    
};



#endif //MODULE_H
