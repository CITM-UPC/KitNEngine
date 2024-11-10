#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "imgui_impl_sdl2.h"
#include "MyWindow.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "Structures/PpMesh.h"
#include "Structures/Shader.h"
#include "Structures/Texture.h"
#include "Component/Camera.h"
#include "Component/GameObject.h"
#include "Config/Config.h"
#include "Core/App.h"
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
	Shader::shaders.push_back(make_shared<Shader>("../Assets/Shaders/MyVertexShader.glsl", "../Assets/Shaders/MyFragmentShader.glsl"));
}


static std::vector<std::shared_ptr<Texture>> textures;

static void draw_mesh(kMeshBase& mesh)
{
	mesh.Render(Shader::shaders[0].get());
	
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw_triangle(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 0.5);
	for (auto& mesh : PpMesh::meshes)
	{
		draw_mesh(*mesh);
	}
}

static bool processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;
		default:
			
			break;
		}
	}
	return true;
}

static bool LoadModels(const char* filename, bool verbose=false)
{
	const aiScene *scene = aiImportFile(filename,aiProcess_Triangulate);

	if (!scene) {
		fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
		return false;
	}

	if (verbose)
		printf("Numero de malles: %i\n", scene->mNumMeshes);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[i];
		if (verbose)
		{ // Soc conscient que tal com està muntat es recorren els vèrtexs dues vegades
			printf("\nMalla %u:\n", i);
			printf(" Numero de vertexs: %u\n", mesh->mNumVertices) ;
			printf(" Numero de triangles: %u\n", mesh->mNumFaces) ;
			// Vèrtexs
			for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
				aiVector3D& vertex = mesh->mVertices[v] ;
				printf(" Vertex %u: (%f, %f, %f)\n", v, vertex.x, vertex.y, vertex.z) ;
			}
			// Índexs de triangles (3 per triangle)
			for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
				aiFace& face = mesh->mFaces[f] ;
				printf(" Indexs triangle %u: ", f) ;
				for (unsigned int j = 0; j < face.mNumIndices; j++) {
					printf("%u ", face.mIndices[j]) ;
				}
				printf("\n") ;
			}
		}
		shared_ptr<PpMesh> pp_mesh =std::make_shared<PpMesh>(mesh);
		if (textures.size() > 0) pp_mesh->texture_id=textures[0]->textureID;
		PpMesh::meshes.emplace_back(pp_mesh);
		
	}
	
	aiReleaseImport(scene);
	return true;
}

static bool LoadTextures(const char* path)
{
	textures.emplace_back(make_shared<Texture>(path));
	return true;
}

void CleanUp()
{
}

AppPtr app = AppPtr(new App);

int main(int argc, char** argv) {
	init_SDL();
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	init_openGL();
	init_devIL();

	init_shaders();
	
	//Temp code (the most permanent type of code)

	// Load textures
	//textures.push_back(make_unique<Texture>(256,256));
	LoadTextures("../Assets/Textures/Baker_house.png");

	//Load models
	//InitDefaultModel();
	const char* path= "../Assets/Models/BakerHouse.fbx";
	
	LoadModels(path, false);

	Camera camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//Temp code end

	app->Init();
	app->Start();
	bool ret = true;
		InitializeGeometryLoading();
		InitializeLibraries();
		for (int p = 0; p < 5; p++) {
			GameObjectPtr parent = std::make_shared<GameObject>(nullptr);
			GameObject::gameObjects.push_back(parent);
			for (int c = 0; c < 3; c++) {
				GameObjectPtr child = std::make_shared<GameObject>(parent);
				parent->GetChildren().push_back(child);
			}
		}

	
	
	while (ret) {
		const auto t0 = hrclock::now();

		ret = app->Update();
		
		camera.update();
		display_func();

		window.swapBuffers();
		const auto t1 = hrclock::now();
		Time::UpdateDeltaTime(last_dt = t1 - t0);
		if(last_dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - last_dt);
	}

	app->CleanUp();
	
	CleanUp();
	
	cout << "Bye World!" << endl;
	return 0;
}
