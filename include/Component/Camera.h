#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;




class Camera
{
public:
    
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    float Yaw;
    float Pitch;
    
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    int lastMouseX = 0, lastMouseY = 0; 
    bool firstMouse = true; 

    float eyex = 0.0;
    float eyey = 0.0;
    float eyez = 0.0;
    float centerx = 0.0;
    float centery = 0.0;
    float centerz = 0.0;
    float upx = 0.0;
    float upy = 0.0;
    float upz = 0.0;

    

    float avance = 0.00000000000000000000001;
    
        
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    void update(Camera_Movement movementDirection, float xoffset = 0.0f, float yoffset = 0.0f, float scrollOffset = 0.0f)
    {
        
        float currentFrame = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        static float lastFrame = 0.0f;
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        
        const Uint8* state = SDL_GetKeyboardState(NULL);

        
        if (state[SDL_SCANCODE_W]) {
            eyex = eyex + avance;
            printf(" Posicion x  %u: ", eyex);
        }
            
        if (state[SDL_SCANCODE_S]) {
            eyex--;
            printf(" Posicion x  %u: ", eyex);
        }
            
        if (state[SDL_SCANCODE_A]) {

        }
           
        if (state[SDL_SCANCODE_D]) {

        }
            

        
        ProcessMouseMovement(xoffset, yoffset);
        
        ProcessMouseScroll(scrollOffset);

        
        updateCameraVectors();
        GetViewMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
        
    }
    void updateMousePosition(int currentMouseX, int currentMouseY) {
        if (firstMouse) {
            
            lastMouseX = currentMouseX;
            lastMouseY = currentMouseY;
            firstMouse = false;
        }

        
        float xoffset = static_cast<float>(currentMouseX - lastMouseX);
        float yoffset = static_cast<float>(lastMouseY - currentMouseY); 

        
        lastMouseX = currentMouseX;
        lastMouseY = currentMouseY;

        
        ProcessMouseMovement(xoffset, yoffset);
    }
    
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
  {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        Yaw += xoffset;
        Pitch += yoffset;
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        
        updateCameraVectors();
    }

    
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    
    void updateCameraVectors()
    {
       
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        Right = glm::normalize(glm::cross(Front, WorldUp));  
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif