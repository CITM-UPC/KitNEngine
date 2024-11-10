#include "Structures/UIWindows.h"
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include "Component/Camera.h"

size_t GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024 * 1024);
    }
    return 0;
}

// Vector para almacenar mensajes de log
std::vector<std::string> logMessages;

// Función para agregar mensajes al log
void AddLogMessage(const std::string& message) {
    logMessages.push_back(message);
}

void ShowConsoleWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);

    if (*p_open && ImGui::Begin("Consola", p_open)) {
        ImGui::TextWrapped("Aqui apareceran los mensajes de la consola...");

        // Mostrar todos los mensajes de log en la consola
        for (const auto& message : logMessages) {
            ImGui::TextWrapped("%s", message.c_str());
        }

        ImGui::End();
    }
}

void ShowConfigWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(420, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    if (*p_open && ImGui::Begin("Configuracion")) {
        static float valores_fps[100] = {};
        static int indice_fps = 0;
        valores_fps[indice_fps] = ImGui::GetIO().Framerate;
        indice_fps = (indice_fps + 1) % 100;
        ImGui::PlotLines("FPS", valores_fps, 100);

        if (ImGui::CollapsingHeader("Configuracion de Modulos")) {
            ImGui::Text("Renderizador, Ventana, Entrada, Texturas...");
        }

        if (ImGui::CollapsingHeader("Informacion del Sistema")) {
            size_t memoriaUso = GetMemoryUsage();
            ImGui::Text("Uso de Memoria: %zu MB", memoriaUso);

            // Reemplazamos "Mondongo" por algo relacionado con la detección de hardware
            ImGui::Text("Deteccion de Hardware:");

            // Aquí podrías agregar más detalles específicos, como por ejemplo:
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            ImGui::Text("Procesador: %u nucleos", si.dwNumberOfProcessors);
            // También puedes mostrar más información de hardware aquí si lo deseas.

            ImGui::Text("Version de OpenGL: 4.3");
        }
        ImGui::End();
    }
}
void ShowInspectorWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

    if (*p_open && ImGui::Begin("Inspector", p_open)) {

        /* if (selectedItem == 0) {  // Si se ha seleccionado la Cámara
            ImGui::Text("Posición de la Cámara:");

            // Comprobamos si activeCamera está válido antes de acceder a position
            
                //glm::vec3 camPos = Camera::activeCamera->position;
                //ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", camPos.x, camPos.y, camPos.z);
        
            
        }
        else if (selectedItem == 1) {  // Si se ha seleccionado "BakedHouse"
            ImGui::Text("GameObject: BakedHouse");

            ImGui::Text("Posición: 0  0  0");
            ImGui::Text("Rotación: 0  0  0");
            ImGui::Text("Escala:  1  1  1");

            ImGui::Text("Número de vértices: 10000");
            ImGui::Text("Número de triángulos: 2000");

            ImGui::Text("Tamaño de la textura: 30x30");
            ImGui::Text("Ruta de la textura: C:\\Users\\Paco\\Documents\\GitHub\\GameEngine\\Assets\\Textures");
        }*/


        ImGui::End();
    }
}


void ShowAboutPopup() {
    if (ImGui::BeginPopup("AboutPopup")) {
        ImGui::Text("Editor del Motor de Juego\nVersión 1.0\nPor Ropuce");
        ImGui::EndPopup();
    }
}

void ShowHerarkiWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    if (*p_open && ImGui::Begin("Jerarquia", p_open)) {

        // Variable para almacenar el estado de la selección
        

        for (GameObjectPtr& g :  GameObject::gameObjects) {
            DisplayGameObjectsInHierarchy(g);
        }
            
            
        ImGui::End();
    }
}


void DisplayGameObjectsInHierarchy(GameObjectPtr& go){
    if (ImGui::TreeNode("GameObject"))
    {

        for (GameObjectPtr& g : go->GetChildren())
        {
            DisplayGameObjectsInHierarchy(g);
        }
        ImGui::TreePop();
    }
}
void InitializeGeometryLoading() {
    AddLogMessage("LOG: Iniciando carga de geometría desde ASSIMP...");
    // Código de carga...
    AddLogMessage("LOG: Carga de geometría completada.");
}

void InitializeLibraries() {
    AddLogMessage("LOG: Iniciando inicialización de bibliotecas externas...");
    // Código de inicialización...
    AddLogMessage("LOG: Inicialización de bibliotecas completada.");
}