//
// Created by Roger on 25/12/2024.
//

#ifndef RESOURCES_H
#define RESOURCES_H
#include <stdexcept>

#include "Modules/Module.h"


class Resources : public Module
{
public:
    Resources();
    bool Init() override;
    bool CleanUp() override;

    template <typename T>
    T Get(const char* filename);

private:

    
    
};

// TODO Acaba aixo
template <typename T>
T Resources::Get(const char* filename)
{
    return T();
}


#endif //RESOURCES_H
