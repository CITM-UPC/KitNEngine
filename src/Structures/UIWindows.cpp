#include "Structures/UIWindows.h"
#include <windows.h>
#include <psapi.h>
#include <sstream>
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

// Funci?n para agregar mensajes al log
void AddLogMessage(const std::string& message) {
    logMessages.push_back(message);
}

void ShowConsoleWindow(bool* p_open) {
    if (!*p_open) return;
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Consola", p_open)) {
        ImGui::TextWrapped("Aqui apareceran los mensajes de la consola...");

        // Mostrar todos los mensajes de log en la consola
        for (const auto& message : logMessages) {
            ImGui::TextWrapped("%s", message.c_str());
        }
        
    }
    ImGui::End();
}

void ShowConfigWindow(bool* p_open) {
    if (!*p_open) return;
    
    ImGui::SetNextWindowPos(ImVec2(420, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Configuracion", p_open)) {
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

            // Reemplazamos "Mondongo" por algo relacionado con la detecci?n de hardware
            ImGui::Text("Deteccion de Hardware:");

            // Aqu? podr?as agregar m?s detalles espec?ficos, como por ejemplo:
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            ImGui::Text("Procesador: %u nucleos", si.dwNumberOfProcessors);
            // Tambi?n puedes mostrar m?s informaci?n de hardware aqu? si lo deseas.

            ImGui::Text("Version de OpenGL: 4.3");
        }
    }
    ImGui::End();
}
void ShowInspectorWindow(bool* p_open) {
    if (!*p_open) return;
    
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Inspector", p_open)) {

        /* if (selectedItem == 0) {  // Si se ha seleccionado la C?mara
            ImGui::Text("Posici?n de la C?mara:");

            // Comprobamos si activeCamera est? v?lido antes de acceder a position
            
                //glm::vec3 camPos = Camera::activeCamera->position;
                //ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", camPos.x, camPos.y, camPos.z);
        
            
        }
        else if (selectedItem == 1) {  // Si se ha seleccionado "BakedHouse"
            ImGui::Text("GameObject: BakedHouse");

            ImGui::Text("Posici?n: 0  0  0");
            ImGui::Text("Rotaci?n: 0  0  0");
            ImGui::Text("Escala:  1  1  1");

            ImGui::Text("N?mero de v?rtices: 10000");
            ImGui::Text("N?mero de tri?ngulos: 2000");

            ImGui::Text("Tama?o de la textura: 30x30");
            ImGui::Text("Ruta de la textura: C:\\Users\\Paco\\Documents\\GitHub\\GameEngine\\Assets\\Textures");
        }*/


    }
    ImGui::End();
}


void ShowAboutPopup() {
    if (ImGui::BeginPopup("AboutPopup")) {
        ImGui::Text("Editor del Motor de Juego\nVersi?n 1.0\nPor Ropuce");
        ImGui::EndPopup();
    }
}

void ShowHerarkiWindow(bool* p_open) {
    if (!*p_open) return;
    
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Jerarquia", p_open)) {
        //ImGui::BeginChild("Scene");        

        for (GameObjectPtr& g :  GameObject::gameObjects) {
            DisplayGameObjectsInHierarchy(g);
        }
            
        //ImGui::EndChild();
    }
    ImGui::End();
}


void DisplayGameObjectsInHierarchy(GameObjectPtr& go){
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    auto nodeName = go->GetName();
    ImGui::PushID(nodeName.c_str());
    
    auto list = go->GetChildren();
    if (list.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (ImGui::TreeNodeEx(nodeName.c_str(),flags))
    {
        if (GameObject::selectedGameObject == go)
        {
            
        }
        else if (ImGui::IsItemClicked())
        {
            AddLogMessage(go->GetName()+" was clicked on Hierarchy");
            GameObject::selectedGameObject = go;
        }
        
        for (GameObjectPtr& g : go->GetChildren())
        {
            DisplayGameObjectsInHierarchy(g);
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}
void InitializeGeometryLoading() {
    AddLogMessage("LOG: Iniciando carga de geometr?a desde ASSIMP...");
    // C?digo de carga...
    AddLogMessage("LOG: Carga de geometr?a completada.");
}

void InitializeLibraries() {
    AddLogMessage("LOG: Iniciando inicializaci?n de bibliotecas externas...");
    // C?digo de inicializaci?n...
    AddLogMessage("LOG: Inicializaci?n de bibliotecas completada.");
}