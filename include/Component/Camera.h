#ifndef CAMERA_H
#define CAMERA_H
#include "Component/Component.h"

#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SDL2/SDL.h>
#include <Config/Config.h>


enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera : public Component
{
public:	// Static members/functions

	static std::shared_ptr<Camera> activeCamera;
	
public:

	
	glm::vec3 worldUp = { 0, 1, 0 };
	glm::vec3 lookTarget;
	
	glm::vec3 position;
	
	glm::vec3 camFront;
	glm::vec3 camRight;
	glm::vec3 camUp;

	
	glm::mat4 view;
	glm::mat4 projection;
	
	//float eyex = -10, eyey = -100, eyez = 0;   // Posición de la cámara
    //float centerx = 0.0f, centery = 10.0f, centerz = 0.0f; // Punto de vista
    //float upx = 0.0f, upy = 1.0f, upz = 0.0f;       // Vector hacia arriba

    float yaw = 90.0f;   // Ángulo horizontal (izquierda-derecha)
    float pitch = 0.0f; // Ángulo vertical (arriba-abajo)
    float moveStep = 10.0f; // Paso de movimiento de la cámara
    float rotationStep = 20.0f; // Paso de rotación de la cámara
    float sensitivity = 100.0f;
    float verticalAngle = 0.0f; // Ángulo vertical para orbitación con W y S

    bool altPressed = false;
	bool shiftPressed = false;
	bool lMousePressed = false;
    bool FPSCam = false;
	bool arcBallCam = false;
    float zoom = 1.0f;
	float zoomStep = 0.2f;
	float targetDistance = 2.0f;
	float speedMulti = 2.0f;

	
	explicit Camera();
	Camera(glm::vec3 pos, glm::vec3 lookAt);

	bool Awake() override;

	bool Start() override;

	bool PreUpdate() override;
	
    bool Update() override;

	bool PostUpdate() override;

	bool InspectorDisplay() override;

	bool CleanUp() override;
    
    void ProcessInput();
  
	void ProcessMouseMovement();
	
	void updateCameraVectors();


	glm::mat4 GetViewProjMatrix() const;

};
#endif