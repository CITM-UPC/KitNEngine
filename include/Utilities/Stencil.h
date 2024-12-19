#ifndef STENCIL_H
#define STENCIL_H

#include "Component/GameObject.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Función para dibujar un objeto 3D resaltando sus bordes
void drawOutlinedObject(const std::shared_ptr<GameObject>& gameObject);

#endif // STENCIL_H