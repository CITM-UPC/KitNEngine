//
// Created by Roger on 8/11/2024.
//

#ifndef INPUT_H
#define INPUT_H
#include "Game/Modules/Module.h"
#include "Game/Utilities/Defs.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include "Game/Structures/Event.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

#define JOYSTICK_MAX 32767
#define DEAD_ZONE 0.05f
#define DEAD_ZONE_INT_MAX (JOYSTICK_MAX-100)
#define DEAD_ZONE_INT_MIN 100

struct SDL_Rect;
class Input;

// Atajo para smart pointers de tipo SDL_GameController
using unique_gameController_t = std::unique_ptr<SDL_GameController, decltype(&SDL_GameControllerClose)>;

// TODO canviar de enum a hashmap o similar per permetre canviar la quantitat de bindings diferents i tenir mes flexibilitat
// Identificadores de controles deben ser añadidos aqui
enum ControlID {
	BINDING_NONE = -1,
	BINDING_CONFIRM,
	BINDING_BACK,
	BINDING_PAUSE,
	BINDING_MOVE_HORIZONTAL,
	BINDING_MOVE_VERTICAL,
	BINDING_UP,
	BINDING_DOWN,
	BINDING_LEFT,
	BINDING_RIGHT,
	BINDING_APP_EXIT,
	BINDING_DEBUG_CONSOLE,
	BINDING_ID_COUNT
};

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

// Contains information about a button and/or axis binding
struct ControlBinding {
public:

	ControlBinding()
	{
		SetDeadZone(DEAD_ZONE);
	}

	ControlBinding(SDL_Scancode _posKey, SDL_GameControllerButton _posButton, SDL_Scancode _negKey, SDL_GameControllerButton _negButton, SDL_GameControllerAxis _axis, float _maxVal, float _deadZone) : posKey(_posKey), posButton(_posButton), negKey(_negKey), negButton(_negButton), axis(_axis), maxVal(_maxVal), deadZone(_deadZone)
	{
		// If any axis-related values is assigned, this binding will update axis data
		isAxisControl = (axis != SDL_CONTROLLER_AXIS_INVALID
			|| negKey != SDL_SCANCODE_UNKNOWN
			|| negButton != SDL_CONTROLLER_BUTTON_INVALID);
			
		SetDeadZone(_deadZone);
	}

	void Update(Input* input);

	void Map(SDL_Scancode& _pKey, SDL_Scancode& _nKey, SDL_GameControllerButton& _pBut, SDL_GameControllerButton& _nBut, bool _isAxis, SDL_GameControllerAxis _axis, ControlID _bind);
	ControlBinding& SetPKey(SDL_Scancode id) { posKey = id; return *this; }
	ControlBinding& SetPButton(SDL_GameControllerButton id) { posButton = id; return *this; }
	ControlBinding& SetNKey(SDL_Scancode id) { negKey = id; return *this; }
	ControlBinding& SetNButton(SDL_GameControllerButton id) { negButton = id; return *this; }
	ControlBinding& SetAxisControl(bool val) { isAxisControl = val; axisVal = 0; return *this; }
	ControlBinding& SetAxis(SDL_GameControllerAxis id) { axis = id; return *this; }
	ControlBinding& SetMaxVal(float val) { maxVal = abs(val); axisVal = MAX(-maxVal, MIN(maxVal, axisVal)); return *this; }
	ControlBinding& SetDeadZone(float val) { deadZone = MAX(DEAD_ZONE_INT_MIN, MIN(DEAD_ZONE_INT_MAX, abs(val)*JOYSTICK_MAX)); return *this; }
	ControlBinding& SetBind(ControlID id) { bind = id; return *this; }
	ControlBinding& SetIsPositive(bool val) { isPositive = val; return *this; }

	// Devuelve el estado del control, combinando teclado y mando
	KeyState State() const { return state; }
	// Si isAxisControl == false siempre devolvera 0
	float Axis() const { return axisVal; }

	void LogData(ControlID id) const;

public:

	bool isAxisControl = false;
	ControlID bind = ControlID::BINDING_NONE;
	bool isPositive = false;

	// Teclas y botones
	SDL_Scancode posKey = SDL_Scancode::SDL_SCANCODE_UNKNOWN;
	SDL_GameControllerButton posButton = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID;

	// Solo para ejes
	SDL_Scancode negKey = SDL_Scancode::SDL_SCANCODE_UNKNOWN;
	SDL_GameControllerButton negButton = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID;
	SDL_GameControllerAxis axis = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_INVALID;

	// Valores
	KeyState state = KeyState::KEY_IDLE;
	float axisVal = 0.0f;
	float maxVal = 1.0f;
	float deadZone = 0; // Set in constructor
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Por defecto devuelve el primer mando, o nullptr si no hay mandos conectados
	SDL_GameController* GetController(glm::uint id = 0) { return (id < controllers.size()) ? controllers[id].get() : nullptr; }

	// Check key states
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Devuelve el estado del boton cuyo id coincide con el proporcionado
	KeyState GetButton(ControlID id);

	// Devuelve un eje
	float GetAxis(ControlID x);
	// Devuelve dos ejes
	glm::vec2 GetAxis(ControlID x, ControlID y);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y) const;
	void GetMouseMotion(int& x, int& y) const;
	void GetMouseWheel(int& x, int& y) const;

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev) const;

	static bool SaveBindings();

	[[nodiscard]] std::string GetText() const { return textInput; }
	void ResetText() { textInput = ""; }

private:

	bool GetKeyRaw(int id) const
	{
		return keyboardRaw[id];
	}

	// Devuelve el estado de un boton, ya sea de teclado o mando
	const ControlBinding& GetBind(ControlID id) const;

	// Actualiza los controles asignados
	void UpdateBindings();

	// Encuentra todos los mandos compatibles conectado al pc
	void FindControllers();

	// Añade el mando con el id proporcionado a la lista de mandos detectados
	void AddController(Sint32 id);


public:

	// Events
	static Event<OnDropEventType&> OnDropFile;
	
private:

	std::string filePath;

	bool windowEvents[WE_COUNT];
	const Uint8* keyboardRaw;
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	std::vector<unique_gameController_t> controllers;
	int	mouseMotionX = 0;
	int mouseMotionY = 0;
	int mouseX = 0;
	int mouseY = 0;
	int mouseWheelX = 0;
	int mouseWheelY = 0;

	// Entrada de texto
	std::string textInput;

	// Input universal (teclado + gamepad)
	std::vector<ControlBinding> bindings;

	friend ControlBinding;

};

#endif // __INPUT_H__


