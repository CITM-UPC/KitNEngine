#ifndef STENCIL_H
#define STENCIL_H

#include "Game/Component/GameObject.h"
#include "Game/Resources/Shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Funci�n para dibujar un objeto 3D resaltando sus bordes
void drawOutlinedObject(const std::shared_ptr<GameObject>& gameObject);


#endif // STENCIL_H