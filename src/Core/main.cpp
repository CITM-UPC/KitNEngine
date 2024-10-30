#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>

#include "imgui_impl_sdl2.h"
#include "MyWindow.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Structures/MyMesh.h"
#include "Structures/Texture.h"
#include "Component/Camera.h"
using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1024, 1024);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

int eyex = -100, eyey = -100, eyez = 0;   // Posición de la cámara
float centerx = 0.0f, centery = 10.0f, centerz = 0.0f; // Punto de vista
float upx = 0.0f, upy = 1.0f, upz = 0.0f;       // Vector hacia arriba

float yaw = 0.0f;   // Ángulo horizontal (izquierda-derecha)
float pitch = 0.0f; // Ángulo vertical (arriba-abajo)
float moveStep = 0.2f; // Paso de movimiento de la cámara
float rotationStep = 2.0f; // Paso de rotación de la cámara
float sensitivity = 0.1f;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
static std::vector<std::unique_ptr<MyMesh>> meshes;
static std::vector<std::unique_ptr<Texture>> textures;

#pragma region MANUAL_MESH

glm::uint vertex_buffer_id = 0;

static const glm::uint num_vertices = 8;
static glm::float32 vertices[num_vertices*3] = {
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f

};

glm::uint index_buffer_id = 0;

const glm::uint num_indices = 36;
static glm::uint  triangleIndices[num_indices]
{
	0,1,2,
	2,3,0,
	
	4,5,6,
	6,7,4,

	4,0,3,
	3,7,4,

	1,5,6,
	6,2,1,

	3,2,6,
	6,7,3,

	4,5,1,
	1,0,4
};

static glm::float32 uvCoords[] = {
	0.0f,0.0f,
	1.0f,0.0f,
	0.0f,1.0f,
	1.0f,1.0f
};

void InitDefaultModel()
{
	vertex_buffer_id = MyMesh::nextBufferId++;
	index_buffer_id = MyMesh::nextBufferId++;
	
	// Generate vertex buffer
	glGenBuffers (1, (GLuint*) &(vertex_buffer_id));
	glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData (GL_ARRAY_BUFFER, sizeof(float)*num_vertices*3, vertices, GL_STATIC_DRAW);

	// Generate vertex_index buffer
	glGenBuffers (1, (GLuint*) &(index_buffer_id));
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*num_indices, triangleIndices, GL_STATIC_DRAW);

	// Generate Texture
	textures.push_back(std::make_unique<Texture>(256,256));
}

#pragma endregion MANUAL_MESH

static void draw_triangle(const u8vec4& color, const vec3& center, double size) {

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	
	glRotatef(0.5f,1.0f,1.0f,1.0f);
	
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	// // Dibuixar una línea 10 unitats amunt
	// glLineWidth(2.0f);
	// glBegin(GL_LINES);
	// glVertex3f(0.f, 0.f, 0.f);
	// glVertex3f(0.f, 10.f, 0.f);
	// glEnd();


	// glColor4ub(color.r, color.g, color.b, color.a);
	// glBegin(GL_TRIANGLES);
	// glVertex3d(center.x, center.y + size, center.z);
	// glVertex3d(center.x - size, center.y - size, center.z);
	// glVertex3d(center.x + size, center.y - size, center.z);
	// glEnd();
}

static void draw_mesh(MyMesh& mesh)
{
	mesh.StartDraw();
	
	mesh.EndDraw();
	//glEnableClientState(GL_VERTEX_ARRAY);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vertex_buffer_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer_id);

	//glRotatef(0.5f, 1.0f, 1.0f, 1.0f);
	//glScalef(0.999f, 0.999f, 0.999f);

	//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw_triangle(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 0.5);
	for (auto& mesh : meshes)
	{
		draw_mesh(*mesh);
	}
	//glRotatef(0.5f, 1.0f, 1.0f, 1.0f);
	glScalef(0.989f, 0.989f, 0.989f);
}

static bool processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;
		default:
			ImGui_ImplSDL2_ProcessEvent(&event);
			break;
		}
	}
	return true;
}
void updateCameraDirection() {
	 
	/*centerx = eyex + cos(pitch) * cos(yaw) * 100.0f;
	centery = eyey + sin(pitch) * 100.0f;
	centerz = eyez + cos(pitch) * sin(yaw) * 100.0f;*/
}

void processEvent(const SDL_Event& event) {
	// Manejar eventos de teclado
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_w: // Mover hacia adelante
			eyex += (centerx - eyex) * moveStep;
			eyey += (centery - eyey) * moveStep;
			eyez += (centerz - eyez) * moveStep;
			updateCameraDirection();
			break;
		case SDLK_s: // Mover hacia atrás
			eyex -= (centerx - eyex) * moveStep;
			eyey -= (centery - eyey) * moveStep;
			eyez -= (centerz - eyez) * moveStep;
			updateCameraDirection();
			break;
		case SDLK_a: // Mover a la izquierda
			eyex += sin(glm::radians(yaw - 90.0f)) * moveStep;
			eyez += -cos(glm::radians(yaw - 90.0f)) * moveStep;
			updateCameraDirection();
			break;
		case SDLK_d: // Mover a la derecha
			eyex += sin(glm::radians(yaw + 90.0f)) * moveStep;
			eyez += -cos(glm::radians(yaw + 90.0f)) * moveStep;
			updateCameraDirection();
			break;
		case SDLK_UP: // Rotar hacia arriba
			pitch += rotationStep;
			if (pitch > 89.0f) pitch = 89.0f; // Limitar el ángulo de pitch
			updateCameraDirection();
			break;
		case SDLK_DOWN: // Rotar hacia abajo
			pitch -= rotationStep;
			if (pitch < -89.0f) pitch = -89.0f;
			updateCameraDirection();
			break;
		case SDLK_LEFT: // Rotar hacia la izquierda
			yaw -= rotationStep;
			updateCameraDirection();
			break;
		case SDLK_RIGHT: // Rotar hacia la derecha
			yaw += rotationStep;
			updateCameraDirection();
			break;
		}
	}
}
void handleMouseMotion(SDL_Event& event) {
	// Movimiento del ratón para rotar la cámara
	yaw += event.motion.xrel * sensitivity;
	pitch -= event.motion.yrel * sensitivity;

	// Limitar pitch para evitar movimientos bruscos al pasar 90 grados
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraDirection();
}

static bool LoadMeshes(const char* filename)
{
	const struct aiScene *scene = aiImportFile(filename,aiProcess_Triangulate);

	if (!scene) {
		fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
		return false;
	}

	printf("Numero de malles: %i\n", scene->mNumMeshes);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[i] ;
		printf("\nMalla %u:\n", i) ;
		printf(" Numero de vertexs: %u\n", mesh->mNumVertices) ;
		printf(" Numero de triangles: %u\n", mesh->mNumFaces) ;
		// Vèrtexs
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			aiVector3D vertex = mesh->mVertices[v] ;
			printf(" Vertex %u: (%f, %f, %f)\n", v, vertex.x, vertex.y, vertex.z) ;
		}
		// Índexs de triangles (3 per triangle)
		for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
			aiFace face = mesh->mFaces[f] ;
			printf(" Indexs triangle %u: ", f) ;
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				printf("%u ", face.mIndices[j]) ;
			}
			printf("\n") ;
		}

		meshes.emplace_back(std::make_unique<MyMesh>(mesh));
	}
	
	aiReleaseImport(scene) ;
	return true;
}

void CleanUp()
{
}


int main(int argc, char** argv) {
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	init_openGL();

	SDL_SetRelativeMouseMode(SDL_TRUE); // Captura el ratón en la ventana

	const char* path = "../Assets/Models/masterchiefSmol.fbx";
	LoadMeshes(path);

	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return 0;
			processEvent(event); // Llama a `processEvent` para manejar las teclas presionadas
		}

		// Actualización de la vista de la cámara
		//Comentar las 3 para ver el modelo de normal
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluPerspective(45.0, (double)WINDOW_SIZE.x / (double)WINDOW_SIZE.y, 1.0, 1000.0);
		gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

		// Renderizado de la escena
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display_func();

		window.swapBuffers();
		std::this_thread::sleep_for(FRAME_DT); // Control de fotogramas por segundo
	}

	CleanUp();
	cout << "¡Adiós, mundo!" << endl;
	return 0;
}
