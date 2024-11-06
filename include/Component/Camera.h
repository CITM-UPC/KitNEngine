#ifndef CAMERA_H
#define CAMERA_H
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <Config/Config.h>

#include "Component.h"


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




class Camera : public Component
{
public:	// Static members/functions

	static std::shared_ptr<Camera> activeCamera;
	
public:

	
	glm::vec3 up = { 0, 1, 0 };
	glm::vec3 position;
	glm::vec3 lookTarget;
	glm::vec3 cameraDirection;
	glm::vec3 camRight;
	glm::vec3 camUp;
	glm::mat4 view;
	//float eyex = -10, eyey = -100, eyez = 0;   // Posición de la cámara
    //float centerx = 0.0f, centery = 10.0f, centerz = 0.0f; // Punto de vista
    //float upx = 0.0f, upy = 1.0f, upz = 0.0f;       // Vector hacia arriba

    float yaw = 0.0f;   // Ángulo horizontal (izquierda-derecha)
    float pitch = 0.0f; // Ángulo vertical (arriba-abajo)
    float moveStep = 0.2f; // Paso de movimiento de la cámara
    float rotationStep = 2.0f; // Paso de rotación de la cámara
    float sensitivity = 0.1f;
    float verticalAngle = 0.0f; // Ángulo vertical para orbitación con W y S

    bool altPressed = false;
    bool rightClickPressed = false;
    float fixedRadius = 100.0f;

	Camera(glm::vec3 pos, glm::vec3 lookAt);
	
    void update();
    
    void ProcessKeyboard(const SDL_Event& event);
  
	void ProcessMouseMovement(const SDL_Event& event);
	
	void updateDirectionVectors();
	
	void updateCameraVectors();
	

};
#endif