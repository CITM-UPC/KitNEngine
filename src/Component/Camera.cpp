#include "Component/Camera.h"

#include "Structures/Shader.h"

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt) : position(pos), lookTarget(lookAt)
{
    updateCameraVectors();
}

void Camera::update()
{
    // TODO moure gestió de tecles a un mòdul dedicat
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
				
        }
				
        ProcessKeyboard(event); // Llama a `processEvent` para manejar las teclas presionadas
    }

    // TODO canviar a matriu inicial + transformacions lineals/afins
    view = glm::lookAt(position,position+camFront,camUp);
    projection = glm::perspective(zoom,(float)WINDOW_SIZE.x/(float)WINDOW_SIZE.y,0.1f,100.0f);
    
    if (Shader::shaders.size() > 0)
    {
        Shader::shaders.at(0)->SetMatrix("view", view);
        Shader::shaders.at(0)->SetMatrix("projection", projection);
    }
    
}

void Camera::ProcessKeyboard(const SDL_Event& event)
{
    // Manejar eventos de teclado para registrar si Alt está presionado
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LALT) {
        altPressed = true;
    }
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LALT) {
        altPressed = false;
    }

    // Manejar eventos del ratón para registrar si el clic derecho está presionado
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
        rightClickPressed = true;
    }
    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {
        rightClickPressed = false;
    }
    if (event.type == SDL_MOUSEMOTION) {
        ProcessMouseMovement(event);
    }
    if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) {
            zoom -= moveStep;
            if (zoom < 0.1f) zoom = 0.1f; 
        }
        else if (event.wheel.y < 0) {
            zoom += moveStep;
            if (zoom > 2.0f) zoom = 2.0f;
        }

        updateCameraVectors();
    }
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_t:
            camUp.z += rotationStep;
            camFront.z += rotationStep;
            break;
        case SDLK_a:
            position -= camRight*moveStep;
            // yaw += rotationStep;
            // updateCameraVectors();
            break;
        case SDLK_d:
            position += camRight*moveStep;
            // yaw -= rotationStep;
            // updateCameraVectors();
            break;
        case SDLK_w:
            position += camFront*moveStep;
            // pitch += rotationStep;
            // updateCameraVectors();
            break;
        case SDLK_s:
            position -= camFront*moveStep;
            // pitch -= rotationStep;
            // updateCameraVectors();
            break;
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
}

void Camera::ProcessMouseMovement(const SDL_Event& event)
{
    if (rightClickPressed) {
        SDL_SetRelativeMouseMode(SDL_TRUE);	
        yaw -= event.motion.xrel * sensitivity;
        pitch += event.motion.yrel * sensitivity; 
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        updateCameraVectors();
    }
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);	
}

void Camera::updateDirectionVectors()
{

}

void Camera::updateCameraVectors()
{
    camFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    camFront.y = sin(glm::radians(pitch));
    camFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camFront = glm::normalize(camFront);
    camRight = glm::normalize(glm::cross(camFront, worldUp));
    camUp = glm::normalize(glm::cross(camRight,camFront));

}
