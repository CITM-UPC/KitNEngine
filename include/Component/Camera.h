#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <Config/Config.h>



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
    
	

	glm::vec3 eye = {-10, -100, 0};
	glm::vec3 center = { 0, 10, 0 };
	glm::vec3 up = { 0, 1, 0 };
	glm::vec3 direcionx = { 0, 0, 0 };
	glm::vec3 direcciony = { 0, 0, 0 };
	glm::vec3 direccionz = { 0, 0, 0 };
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
    
    void update()
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
        gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
        
    }
    void ProcessKeyboard(const SDL_Event& event)
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
				center.z += rotationStep;
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
				eye.x += rotationStep;
				center.x += rotationStep;
				
				break;
			case SDLK_DOWN:
				eye.x -= rotationStep;
				center.x -= rotationStep;
				
				break;
			case SDLK_LEFT:
				eye.z -= rotationStep;
				center.z -= rotationStep;
				break;
			case SDLK_RIGHT:
				eye.z += rotationStep;
				center.z += rotationStep;
				break;
			case SDLK_f:
				center.x == 0;
				center.y == 0;
				center.z == 0;
				eye.x == 0;
				eye.y == 0;
				eye.z == 0;
				break;
			}
		}
    }
	void ProcessMouseMovement(const SDL_Event& event)
	{
		if (altPressed && rightClickPressed) {
			yaw += event.motion.xrel * sensitivity;
			pitch -= event.motion.yrel * sensitivity; 
			/*if (pitch > 89.0f) pitch = 89.0f;
			if (pitch < -89.0f) pitch = -89.0f;*/
			updateCameraVectors();
		}
	}
	void updateDirectionVectors() {

	}
	void updateCameraVectors()
	{
		eye.x = center.x + fixedRadius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		eye.y = center.y + fixedRadius * sin(glm::radians(pitch));
		eye.z = center.z + fixedRadius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
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

};
#endif