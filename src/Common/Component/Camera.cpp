#include "Game/Component/Camera.h"

#include "Game/Component/GameObject.h"
#include "Game/Resources/Shader.h"
#include "Game/Modules/Input.h"
#include "Game/Core/Game.h"
#include "Game/Utilities/Time.h"

#include "Game/Config/Config.h"


std::shared_ptr<Camera> Camera::activeCamera;

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
}

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt) : Component("Camera"), lookTarget(lookAt), position(pos)
{
}

bool Camera::Awake()
{
    if (activeCamera == nullptr) activeCamera = std::dynamic_pointer_cast<Camera>(GetSmartPtr());
    targetDistance = glm::distance(position,lookTarget);
    updateCameraVectors();
    return true;
}

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
    TransformPtr t = GetGameObject()->GetTransform();
    position = t->GetPosition();
    camFront = -t->GetForward();
    camRight = t->GetRight();
    camUp = t->GetUp();

    ProcessInput();
    
    //view = glm::lookAt(position,position+camFront,camUp);
    view = glm::lookAt(position,position+camFront,worldUp);
    t->SetRotation(view);
    t->SetPosition(position);
    
    projection = glm::perspective(zoom,(float)WINDOW_SIZE.x/(float)WINDOW_SIZE.y,0.1f,100.0f);

    if (activeCamera.get() == this && !Shader::shaders.empty())
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
    altPressed = game->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT;

    shiftPressed = game->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT;
    
    // Manejar eventos del ratón para registrar si el clic derecho está presionado
    KeyState rButtonState = game->input->GetMouseButtonDown(SDL_BUTTON_RIGHT);
    FPSCam = rButtonState == KeyState::KEY_REPEAT;


    lMousePressed = game->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT;
    arcBallCam = altPressed && lMousePressed;
    
    ProcessMouseMovement();

    if (game->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
    {
        lookTarget = glm::vec3(0.0f,0.0f,0.0f);
        targetDistance = glm::distance(position,lookTarget);
        arcBallCam = true; // Aprofita el codi de arcball que centra la camera sobre l'objectiu
    }

    int wheelX, wheelY;
    game->input->GetMouseWheel(wheelX, wheelY);
    

    if (FPSCam)
    {
        //TODO Moviment independent de framerate (lerp)
        // Moviment camara fps
        if (game->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
            position += camFront * moveStep * (shiftPressed ? speedMulti : 1) * Time::GetDeltaTime();
        if (game->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
            position -= camFront * moveStep * (shiftPressed ? speedMulti : 1) * Time::GetDeltaTime();
        if (game->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
            position -= camRight * moveStep * (shiftPressed ? speedMulti : 1) * Time::GetDeltaTime();
        if (game->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
            position += camRight * moveStep * (shiftPressed ? speedMulti : 1) * Time::GetDeltaTime();
      
    }
    else
    {
        // Moviment orbital
        targetDistance -= (wheelY * zoomStep); // Apropa o allunya de l'objecte
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
    if (FPSCam || arcBallCam) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        int x,y;
        game->input->GetMouseMotion(x,y);
        yaw += x * sensitivity*0.01f; // * Time::GetDeltaTime();
        pitch -= y * sensitivity*0.01f; // * Time::GetDeltaTime(); 
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        //updateCameraVectors();
    }
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);	
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

glm::mat4 Camera::GetViewProjMatrix() const
{
    return projection * GetGameObject()->GetTransform()->GetBasis();
}
