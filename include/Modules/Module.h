//
// Created by Roger on 02/10/2024.
//

#ifndef MODULE_H
#define MODULE_H
#include <string>


class Module {
public:
    
    Module(const char* name) : _name(name)
    {
        
    }

    virtual ~Module();    

    bool Init();

    bool Start();

    bool Update();

    bool CleanUp();

    const char* GetName() const { return _name.c_str(); }

    
private:

    std::string _name;
    
};



#endif //MODULE_H
