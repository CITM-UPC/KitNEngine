#include <GL/glew.h>
#include <chrono>
#include <memory>
#include <thread>
#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "MyWindow.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "Structures/MeshRenderer.h"
#include "Structures/Shader.h"
#include "Structures/Texture.h"
#include "Component/GameObject.h"
#include "Component/Camera.h"
#include "Config/Config.h"
#include "Editor/Core/Editor.h"
#include "Game/Core/Game.h"
#include "Utilities/Time.h"
#include "Structures/UIWindows.h"

using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;


static constexpr unsigned int FPS = 60;
static constexpr auto FRAME_DT = 1.0s / FPS;
static chrono::duration<float> last_dt = 0s;

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "OpenGL Debug Message: " << message << std::endl;
}

static void init_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}

}

static void init_openGL() {
	glewExperimental = GL_TRUE;
	glewInit();
	
	if (!GLEW_VERSION_3_3) throw exception("OpenGL 3.3 API is not available.");
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ARB_shading_language_420pack);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	
	// Enable debugging output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Ensures debugging is synchronous
	glDebugMessageCallback(GLDebugMessageCallback, nullptr);

	
}

void init_devIL()
{
	ilInit();
	iluInit();
	//ilutInit();

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		throw exception("DevIL version mismatch");
	}
}

void init_shaders()
{
	Shader::shaders.push_back(make_shared<Shader>("Assets/Shaders/MyVertexShader.glsl", "Assets/Shaders/MyFragmentShader.glsl"));
}

/*static void draw_mesh(kRendererBase& mesh)
{
	mesh.Render(Shader::shaders[0].get());
	
}*/


static bool LoadModels(const char* filename)
{
	MeshRenderer::ImportMeshes(filename, true);
	return true;
}

static bool LoadTextures(const char* path)
{
	Texture::ImportTexture(path);
	return true;
}

void CleanUp()
{
}

GamePtr game = std::make_unique<Game>();
EditorPtr editor = std::make_unique<Editor>();

int main(int argc, char** argv) {
	init_SDL();
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	init_openGL();
	init_devIL();

	init_shaders();
	
	//Temp code (the most permanent type of code)

	// Load textures
	//textures.push_back(make_unique<Texture>(256,256));
	LoadTextures("Assets/Textures/Street environment/Building_V02_C.png");

	//Load models
	//InitDefaultModel();
	const char* path= "Assets/Models/Street environment_V01.FBX";
	
	LoadModels(path);

	/*std::shared_ptr<GameObject> modelRoot = GameObject::CreateGameObject(nullptr, "mesh");
	for (auto& mesh : MeshRenderer::renderers)
	{
		std::shared_ptr<GameObject> g = GameObject::CreateGameObject(modelRoot, "GameObject "+mesh->GetName());
		//auto c = dynamic_pointer_cast<Component>(mesh);
		g->AddComponent(mesh);
	}*/

	// GameObject arrel per a components de l'editor. No es troba en la llista principal de GameObjects
	/*std::shared_ptr<GameObject> editorRootObject = make_shared<GameObject>("EditorObject");

	// Creacio generica de component
	Camera::activeCamera = editorRootObject->AddComponentOfType<Camera>(glm::vec3(0.0f, 0.0f, -3.0f),
	                                                                    glm::vec3(0.0f, 1.0f, 0.0f));
	
	
	editorRootObject->Awake();
	editorRootObject->Start();*/

	//Temp code end

	game->Init();
	editor->Init();
	game->Start();
	editor->Start();
	
	bool ret = true;
	for (int p = 0; p < 5; p++)
	{
		std::shared_ptr<GameObject> parent = GameObject::CreateGameObject(nullptr,"Parent "+std::to_string(p));
		for (int c = 0; c < 3; c++) {
			std::shared_ptr<GameObject> child = GameObject::CreateGameObject(parent,"Child "+std::to_string(c));
		}
	}

	
	
	while (ret) {
		const auto t0 = hrclock::now();
		window.StartFrame();

		ret &= game->Update();
		ret &= editor->Update();

		window.swapBuffers();
		const auto t1 = hrclock::now();
		Time::UpdateDeltaTime(last_dt = t1 - t0);
		if(last_dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - last_dt);
	}

	game->CleanUp();
	
	CleanUp();
	
	cout << "Bye World!" << endl;
	return 0;
}
