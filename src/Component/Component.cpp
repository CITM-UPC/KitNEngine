//
// Created by Roger on 11/11/2024.
//
#include "Component/Component.h"

#include <stdexcept>

bool Component::Enable()
{
    if (!_enabled)
    {
        _enabled = true;
    }
    return _enabled;
}

bool Component::Disable()
{
    if (_enabled)
    {
        _enabled = false;
    }
    return _enabled;
}
