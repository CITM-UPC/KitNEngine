#include "Component/Camera.h"

#include <ostream>

#include "Core/App.h"
#include "Structures/Shader.h"

#include "Modules/Input.h"

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
}

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt) : Component(), lookTarget(lookAt), position(pos)
{
    targetDistance = glm::distance(position,lookTarget);
    updateCameraVectors();
}

bool Camera::Awake()
{
    return true;
}

std::shared_ptr<Camera> Camera::activeCamera;

bool Camera::Start()
{
    return Component::Start();
}

bool Camera::PreUpdate()
{
    return Component::PreUpdate();
}

bool Camera::Update()
{
    ProcessInput();

    // TODO canviar a matriu inicial + transformacions lineals/afins
    // TODO Canviar a utilitzar Transform del GameObject al que esta assignada
    view = glm::lookAt(position,position+camFront,camUp);
    projection = glm::perspective(zoom,(float)WINDOW_SIZE.x/(float)WINDOW_SIZE.y,0.1f,100.0f);
    
    if (!Shader::shaders.empty())
    {
        Shader::shaders.at(0)->SetMatrix("view", view);
        Shader::shaders.at(0)->SetMatrix("projection", projection);
    }

    return Component::Update();
}

bool Camera::PostUpdate()
{
    return Component::PostUpdate();
}

bool Camera::InspectorDisplay()
{
    return Component::InspectorDisplay();
}

bool Camera::CleanUp()
{
    return Component::CleanUp();
}

void Camera::ProcessInput()
{
    SDL_Event event = SDL_Event();
    
    // TODO tecles configurables (ja implementat a modul Input)
    
    // Manejar eventos de teclado para registrar si Alt está presionado
    altPressed = app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT;

    shiftPressed = app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT;
    
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
            position += camFront * moveStep * (shiftPressed ? speedMulti : 1);
        if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
            position -= camFront * moveStep * (shiftPressed ? speedMulti : 1);
        if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
            position -= camRight * moveStep * (shiftPressed ? speedMulti : 1);
        if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
            position += camRight * moveStep * (shiftPressed ? speedMulti : 1);
      
    }
    else
    {
        // Moviment orbital
        targetDistance -= (wheelY * moveStep); // Apropa o allunya de l'objecte
        if (targetDistance < 0.1f) targetDistance = 0.1f;
        if (targetDistance > 100.0f) targetDistance = 100.0f;

        arcBallCam = true; // Aprofita càmera orbital per moure al nou punt
    }
    // else
    // {
    //     zoom -= wheelY * moveStep;
    //     if (zoom < 0.1f) zoom = 0.1f;
    //     else if (zoom > 3.0f) zoom = 3.0f;
    // }
    
    
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
