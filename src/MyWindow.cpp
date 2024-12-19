#include <SDL2/SDL.h>
#include "MyWindow.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <iostream>
#include <Component/Formas Primitivas.h>
#include <Structures/UIWindows.h>

#include "Editor/Core/Editor.h"
#include "Modules/Inspector.h"
using namespace std;

MyWindow::MyWindow(const std::string& title, int w, int h) : _width(w), _height(h) {

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Quita retrocompatibilidad
#ifdef _DEBUG
    std::cout<< "Creating debug context" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif _DEBUG
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
    if (!_window) throw exception(SDL_GetError());

    _ctx = SDL_GL_CreateContext(_window);
    if (!_ctx) throw exception(SDL_GetError());
    if (SDL_GL_MakeCurrent(_window, _ctx) != 0) throw exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(_window, _ctx);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

MyWindow::~MyWindow() {
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    
    SDL_GL_DeleteContext(_ctx);
    SDL_DestroyWindow(static_cast<SDL_Window*>(_window));
}

void MyWindow::StartFrame() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void MyWindow::swapBuffers() const {
    static bool showConsole = false;
    static bool showConfig = false;
    static bool showInspector = false;
    static bool showherarqui = false;


    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Archivo")) {
            if (ImGui::MenuItem("Salir")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
            if (ImGui::MenuItem("GitHub")) {
                std::system("start https://github.com/Ropuce/GameEngine");
            }
            if (ImGui::MenuItem("Acerca de")) {
                ImGui::OpenPopup("AboutPopup");
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Cargar Primitivas")) {
            if (ImGui::MenuItem("Cargar Circulo")) draw_circle();
            if (ImGui::MenuItem("Cargar Cubo")) draw_cube();
            if (ImGui::MenuItem("Cargar Triangulo")) draw_triangle();
            if (ImGui::MenuItem("Cargar Cilindro")) draw_cylinder();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Ventanas del Editor")) {
            if (ImGui::MenuItem("Consola", NULL, &showConsole))
                cout << "ConsolaItem pulsado" << endl;
            ImGui::MenuItem("Configuracion", NULL, &showConfig);
            ImGui::MenuItem("Inspector", NULL, &showInspector);
            ImGui::MenuItem("Jerarquia", NULL, &showherarqui);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    editor->inspector->SetState(showInspector);
    

    ShowConsoleWindow(&showConsole);
    ShowConfigWindow(&showConfig);
    //ShowInspectorWindow(&showInspector);
    ShowHerarkiWindow(&showherarqui);
    
    ShowAboutPopup();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(_window));
}
