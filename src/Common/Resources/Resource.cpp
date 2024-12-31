//
// Created by Roger on 31/12/2024.
//
#include "Game/Resources/Resource.h"
#include "Game/Modules/ResourceManager.h"

bool Resource::InUse()
{
    // Mala implementacio, requereix tornar a cercar l'element. Ideal seria un weak_ptr membre de l'objecte per tenir acces directe al comptador
    auto thisPtr = ResourceManager::getInstance()->Get<Resource>(r_name);
    return thisPtr.use_count() > 1;
}