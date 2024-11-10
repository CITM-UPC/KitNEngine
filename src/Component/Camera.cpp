#include "Component/Camera.h"

#include <ostream>

#include "Core/App.h"
#include "Structures/Shader.h"

#include "Modules/Input.h"

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt) : position(pos), lookTarget(lookAt)
{
    targetDistance = glm::distance(position,lookTarget);
    updateCameraVectors();

}
std::shared_ptr<Camera> Camera::activeCamera;
void Camera::update()
{
    ProcessInput();

    // TODO canviar a matriu inicial + transformacions lineals/afins
    view = glm::lookAt(position,position+camFront,camUp);
    projection = glm::perspective(zoom,(float)WINDOW_SIZE.x/(float)WINDOW_SIZE.y,0.1f,100.0f);
    
    if (!Shader::shaders.empty())
    {
        Shader::shaders.at(0)->SetMatrix("view", view);
        Shader::shaders.at(0)->SetMatrix("projection", projection);
    }
    
}

void Camera::ProcessInput()
{
    SDL_Event event = SDL_Event();
    
    // TODO tecles configurables (ja implementat a modul Input)
    
    // Manejar eventos de teclado para registrar si Alt está presionado
    altPressed = app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT;
    
    // Manejar eventos del ratón para registrar si el clic derecho está presionado
    KeyState rButtonState = app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT);
    FPSCam = rButtonState == KeyState::KEY_REPEAT;


    rMousePressed = app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT;
    arcBallCam = altPressed && rMousePressed;
    
    ProcessMouseMovement();

    if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
    {
        lookTarget = glm::vec3(0.0f,0.0f,0.0f);
        targetDistance = glm::distance(position,lookTarget);
        arcBallCam = true; // Aprofita el codi de arcball que centra la camera sobre l'objectiu
    }

    int wheelX, wheelY;
    app->input->GetMouseWheel(wheelX, wheelY);
    

    if (FPSCam)
    {
        // Moviment camara fps
        if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
            position += camFront*moveStep;
        if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
            position -= camFront*moveStep;
        if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
            position -= camRight*moveStep;
        if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
            position += camRight*moveStep;
      
    }
    else if (arcBallCam)
    {
        // Moviment orbital
        targetDistance -= (wheelY * moveStep); // Apropa o allunya de l'objecte
    }
    else
    {
        zoom -= wheelY * moveStep;
    }
        
    
    
    /*
     *if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            position.x += moveStep;
            lookTarget.x += moveStep;
				
            break;
        case SDLK_DOWN:
            position.x -= moveStep;
            lookTarget.x -= moveStep;
				
            break;
        case SDLK_LEFT:
            position.z -= moveStep;
            lookTarget.z -= moveStep;
            break;
        case SDLK_RIGHT:
            position.z += moveStep;
            lookTarget.z += moveStep;
            break;
        case SDLK_f:
            lookTarget.x = 0;
            lookTarget.y = 0;
            lookTarget.z = 0;
            position.x = 0;
            position.y = 0;
            position.z = 1;
            break;
        }
    }
    */
    
    updateCameraVectors();

}

void Camera::ProcessMouseMovement()
{
    if (FPSCam || (arcBallCam && rMousePressed)) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        int x,y;
        app->input->GetMouseMotion(x,y);
        yaw += x * sensitivity;
        pitch -= y * sensitivity; 
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        //updateCameraVectors();
    }
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);	
}

void Camera::updateDirectionVectors()
{

}

void Camera::updateCameraVectors()
{
    if (arcBallCam)
    {
        position.x = lookTarget.x - targetDistance * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        position.y = lookTarget.y - targetDistance * sin(glm::radians(pitch));
        position.z = lookTarget.z - targetDistance * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        
        camFront = lookTarget - position;
    }
    else
    {
        
        camFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        camFront.y = sin(glm::radians(pitch));
        camFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));    
    }

    camFront = glm::normalize(camFront);
    
    camRight = glm::normalize(glm::cross(camFront, worldUp));
    camUp = glm::normalize(glm::cross(camRight,camFront));

    if (FPSCam)
    {
        targetDistance = glm::distance(position,lookTarget);
        lookTarget = position+camFront*targetDistance;
    }

}
