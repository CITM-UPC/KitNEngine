#include "Component/Camera.h"

#include <stdexcept>

#include "Structures/Shader.h"

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt) : position(pos), lookTarget(lookAt)
{
    cameraDirection = glm::normalize(position-lookTarget);
    camRight = glm::normalize(glm::cross(up, cameraDirection));
    camUp = glm::cross(cameraDirection, camRight);
    view = glm::lookAt(position,lookTarget, up);
}

void Camera::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
				
        }
				
        ProcessKeyboard(event); // Llama a `processEvent` para manejar las teclas presionadas
    }
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluPerspective(45.0, (double)WINDOW_SIZE.x / (double)WINDOW_SIZE.y, 1.0, 1000.0);
    gluLookAt(position.x, position.y, position.z, lookTarget.x, lookTarget.y, lookTarget.z, up.x, up.y, up.z);
    //view = glm::lookAt(position,lookTarget,up);
    if (Shader::shaders.size() > 0)
        Shader::shaders.at(0)->SetViewMatrix(view);
    
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
            fixedRadius -= 3.0f;
            if (fixedRadius < 1.0f) fixedRadius = 1.0f; 
        }
        else if (event.wheel.y < 0) {
            fixedRadius += 3.0f;
        }

        updateCameraVectors();
    }
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_t:
            up.z += rotationStep;
            lookTarget.z += rotationStep;
            break;
        case SDLK_a:
            yaw -= rotationStep;
            updateCameraVectors();
            break;
        case SDLK_d:
            yaw += rotationStep;
            updateCameraVectors();
            break;
        case SDLK_w:
            pitch -= rotationStep;
            updateCameraVectors();
            break;
        case SDLK_s:
            pitch += rotationStep;
            updateCameraVectors();
            break;
        case SDLK_UP:
            position.x += rotationStep;
            lookTarget.x += rotationStep;
				
            break;
        case SDLK_DOWN:
            position.x -= rotationStep;
            lookTarget.x -= rotationStep;
				
            break;
        case SDLK_LEFT:
            position.z -= rotationStep;
            lookTarget.z -= rotationStep;
            break;
        case SDLK_RIGHT:
            position.z += rotationStep;
            lookTarget.z += rotationStep;
            break;
        case SDLK_f:
            lookTarget.x = 0;
            lookTarget.y = 0;
            lookTarget.z = 0;
            position.x = 0;
            position.y = 0;
            position.z = 0;
            break;
        }
    }
}

void Camera::ProcessMouseMovement(const SDL_Event& event)
{
    if (altPressed && rightClickPressed) {
        yaw += event.motion.xrel * sensitivity;
        pitch -= event.motion.yrel * sensitivity; 
        /*if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;*/
        updateCameraVectors();
    }
}

void Camera::updateDirectionVectors()
{

}

void Camera::updateCameraVectors()
{
    position.x = lookTarget.x + fixedRadius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = lookTarget.y + fixedRadius * sin(glm::radians(pitch));
    position.z = lookTarget.z + fixedRadius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    /*Front = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));
    glm::vec3 right = glm::normalize(glm::cross(Front, glm::vec3(up.x, up.y, up.z)));
    glm::vec3 upt = glm::normalize(glm::cross(right, Front));
    up.x = upt.x;
    up.y = upt.y;
    up.z = upt.z;*/
}
