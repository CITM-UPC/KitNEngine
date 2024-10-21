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
using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1024, 1024);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

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
	glRotatef(0.5f, 1.0f, 1.0f, 1.0f);
	glScalef(0.999f, 0.999f, 0.999f);
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

	
	//Temp code (the most permanent type of code)
	const char* path= "../Assets/Models/masterchiefSmol.fbx";
	
	LoadMeshes(path);
	//Temp code end

	
	
	while (processEvents()) {
		const auto t0 = hrclock::now();
		display_func();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if(dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}

	CleanUp();
	
	cout << "Bye World!" << endl;
	return 0;
}
